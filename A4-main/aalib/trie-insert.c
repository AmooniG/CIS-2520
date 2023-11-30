#include <stdio.h>
#include <string.h> // for strcmp()
#include <ctype.h> // for isprint()
#include <stdlib.h> // for malloc()
#include <assert.h>
#include <stdbool.h>
#include "trie_defs.h"


/** compare a key with the letter in a node */
int
trie_subtreeSearchComparator(const void *keyValue, const void *nodePtr)
{
	TrieNode **node = (TrieNode **) nodePtr;
	AAKeyType key = (AAKeyType) keyValue;
	return (key[0] - (*node)->letter);
}

/** create a whole chain for the rest of the key */
static TrieNode *
trie_create_chain(AAKeyType key, size_t keylength, void *value, int *cost)
{
    TrieNode *current = NULL, *head = NULL;

	// TO DO: create a new chain the the required key letters
    for (size_t i = 0; i < keylength; i++) {
        TrieNode *newNode = malloc(sizeof(TrieNode));
        newNode->letter = key[i];
        newNode->value = (i == keylength - 1) ? value : NULL;
        newNode->nSubtries = 0;
        newNode->subtries = NULL;
        if (cost) (*cost)++;
        if (current) {
            current->subtries = realloc(current->subtries, (current->nSubtries + 1) * sizeof(TrieNode *));
            current->subtries[current->nSubtries++] = newNode;
        } else {
            head = newNode;
        }
        current = newNode;
    }
    return head;
}

/** add the given chain to the list of tries in the current node */
static int
trie_add_chain(
		TrieNode **subtreeList, int nSubtries,
		AAKeyType key,
		TrieNode *newChain
	)
	
{
	// TO DO:  add the provided chain to the node; the first letter
	// of the key may be used to organize where within the set of
	// subtries you add this
	int insertPos = 0;
    while (insertPos < nSubtries && subtreeList[insertPos]->letter < key[0]) {
        insertPos++;
    }

    subtreeList = realloc(subtreeList, (nSubtries + 1) * sizeof(TrieNode *));

    for (int i = nSubtries; i > insertPos; i--) {
        subtreeList[i] = subtreeList[i - 1];
    }

    subtreeList[insertPos] = newChain;
	// TO DO: you probably want to replace this return statement
	// with your own code
    return 0; // Return 0 on successful addition
}


/** link the provided key into the current chain */
static int
trie_link_to_chain(TrieNode *current,
		AAKeyType key, size_t keylength,
		void *value, int *cost)
{
	// TO DO: add the remaining portions of the key
	// into this chain, forming a new branch if and when
	// they stop matching existing letters within the subtries
    for (size_t i = 0; i < keylength; i++) {
        bool found = false;
        for (int j = 0; j < current->nSubtries; j++) {
            if (current->subtries[j]->letter == key[i]) {
                current = current->subtries[j];
                found = true;
                if (cost) (*cost)++;
                break;
            }
        }
        if (!found) {
            current->subtries = realloc(current->subtries, (current->nSubtries + 1) * sizeof(TrieNode *));
            TrieNode *newNode = malloc(sizeof(TrieNode));
            newNode->letter = key[i];
            newNode->value = (i == keylength - 1) ? value : NULL;
            newNode->nSubtries = 0;
            newNode->subtries = NULL;
            current->subtries[current->nSubtries++] = newNode;
            current = newNode;
        }
    }
    return 0;
}

int
trieInsertKey(KeyValueTrie *root,
		AAKeyType key, size_t keylength,
		void *value, int *cost)
{
	/** keep the max key length in order to keep a buffer for interation */
    if (root->maxKeyLength < keylength) {
        root->maxKeyLength = keylength;
    }

    if (root->nSubtries == 0) {
        root->subtries = malloc(sizeof(TrieNode *));
        root->subtries[root->nSubtries++] = trie_create_chain(key, keylength, value, cost);
        return 0;
    }

	
	// TO DO: find the subtrie with the leading letter of the
	// key, and insert the new key into the correct subtrie
	// chain based on that letter
    for (size_t i = 0; i < keylength; i++) {
        TrieNode *current = root->subtries[0];
        bool found = false;
        for (int j = 0; j < current->nSubtries; j++) {
            if (current->subtries[j]->letter == key[i]) {
                if (i == keylength - 1) {
                    current->subtries[j]->value = value;
                    return 0;
                }
                current = current->subtries[j];
                found = true;
                if (cost) (*cost)++;
                break;
            }
        }
        if (!found) {
            trie_link_to_chain(current, &key[i], keylength - i, value, cost);
            return 0;
        }
    }

    return -1;
}
