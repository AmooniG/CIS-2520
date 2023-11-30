#include <stdio.h>
#include <string.h> // for strcmp()
#include <ctype.h> // for isprint()
#include <stdlib.h> // for malloc()
#include <assert.h>
#include <stdbool.h>
#include "trie_defs.h"

/** find a key within the trie */
void *trieLookupKey(
        KeyValueTrie *root,
        AAKeyType key, size_t keylength,
        int *cost
    )
{
    if (root == NULL || root->nSubtries == 0) {
        return NULL; // Early return if root is NULL or has no subtries
    }

    TrieNode *current = root->subtries[0]; // Declare 'current' outside the loop

    // TO DO: walk the trie to find the key, returning
    // a value if there is one after we have finished
    // our walk
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
            // Key not found in the trie
            return NULL;
        }
    }

    // Check if the final node has a value
    if (current != NULL && current->value != NULL) {
        return current->value;
    }

    /** return null if the node doesn't have a value */
    return NULL;
}
