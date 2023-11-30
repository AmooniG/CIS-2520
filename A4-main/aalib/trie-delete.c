#include <stdio.h>
#include <string.h> // for strcmp()
#include <ctype.h> // for isprint()
#include <stdlib.h> // for malloc()
#include <assert.h>

#include "trie_defs.h"


/** recursively roll up the chain */
static int
walk_chain_to_delete(
		void **value,
		TrieNode *curSearchNode,
		AAKeyType key, size_t keylength,
		int *cost
	)
{
	// TO DO: remove nodes for deleted key
    if (keylength == 0 || curSearchNode == NULL) {
        return -1; // Key not found or end of chain
    }

    if (curSearchNode->letter == key[0]) {
        if (keylength == 1) {
            // Last letter of the key, remove the value
            *value = curSearchNode->value;
            curSearchNode->value = NULL;
            if (cost) (*cost)++;
            return 0; // Key found and value removed
        } else {
            // Continue searching down the chain
            return walk_chain_to_delete(value, curSearchNode->subtries[0], key + 1, keylength - 1, cost);
        }
    } else {
        // Continue searching in the next subtrie
        for (int i = 0; i < curSearchNode->nSubtries; i++) {
            if (walk_chain_to_delete(value, curSearchNode->subtries[i], key, keylength, cost) == 0) {
                return 0; // Key found and value removed in a subtrie
            }
        }
    }

    return -1; // Key not found in this path
}

/** delete a key from the trie */
void *trieDeleteKey(
		KeyValueTrie *root,
		AAKeyType key, size_t keylength,
		int *cost
	)
{
	void *valueFromDeletedKey = NULL;


	if (root->nSubtries == 0) {
		return NULL;
	}

	/**
	 ** TO DO: search for the right subchain and delete the key from it
	 **/
	    for (int i = 0; i < root->nSubtries; i++) {
        if (walk_chain_to_delete(&valueFromDeletedKey, root->subtries[i], key, keylength, cost) == 0) {
            return valueFromDeletedKey; // Key found and value removed
        }
    }
	
	return valueFromDeletedKey;
}

