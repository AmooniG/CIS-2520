#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LLNode.h"
#include "word_extractor.h"

int printData(char *filename, LLNode **wordListHeads, int maxLen);
int printHapax(char *filename, LLNode **wordListHeads, int maxLen, int hapaxLength);

// Function to tally words in a file
int tallyWordsInFile(char *filename, LLNode **wordLists, int maxLen) {
	//creating a word extractor for a specific given file
    struct WordExtractor *we = weCreateExtractor(filename, maxLen);
    if (!we) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return -1;
    }
	int wordCount = 0; // setting word count to 0

	//while loop to go through each word in the file
    while (weHasMoreWords(we)) {
        char *word = weGetNextWord(we);
        int len = strlen(word);
        if (len > maxLen) {
            continue;  // if length of a word is over the maximum it skips it
        }

		//checks if word already exists
        LLNode *node = llLookupKey(wordLists[len], word);
        if (node) {
			// Increment count for existing word
            node->value++;  
        } else {
            // Add new word to the list with count 1
            node = llNewNode(strdup(word), 1);
            wordLists[len] = llAppend(wordLists[len], node);
        }
		wordCount++;
    }
	//cleanup (deletes the extractor)
    weDeleteExtractor(we);
    return wordCount;
}

// Function to print all data loaded
int printData(char *filename, LLNode **wordListHeads, int maxLen) {
    printf("Data from file '%s':\n", filename);
	// a for loop to go through words of all lengths up to maximum length
    for (int i = 0; i <= maxLen; i++) {
        LLNode *current = wordListHeads[i];
        while (current) {
            printf("%s: %d\n", current->key, current->value);
            current = current->next;
        }
    }
    return 0;
}

// Function to print hapax legomena
int printHapax(char *filename, LLNode **wordListHeads, int maxLen, int hapaxLength) {
    printf("Hapax legomena from file '%s':\n", filename);
	// a for loop to go through words of all lengths up to maximum length
    for (int i = 0; i <= maxLen; i++) {
        if (hapaxLength == 0 || hapaxLength == i) {
            LLNode *current = wordListHeads[i];
            while (current) {
				//if words appear only once its a hapax legomenon
                if (current->value == 1) {
                    printf("%s\n", current->key);
                }
                current = current->next; //move to next node
            }
        }
    }
    return 0;
}
