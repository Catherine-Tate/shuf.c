#include "epath.h"

#include <bsd/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// expects char * of space seperated values taken from the arguments
char **makeLines(char *input, int *count) {
  char **allWords = NULL;
  // char * inputBak = input;
  int numWords = 0;
  char *word;

  word = strtok(input, " ");
  if (word != NULL) {
    numWords++;
    allWords = malloc(sizeof(char *) * numWords);
    allWords[0] = malloc((sizeof(char) * strlen(word)) + 1);
    strlcpy(allWords[0], word, strlen(word));
  }

  while (word != NULL) {
    numWords++;
    allWords = realloc(allWords, sizeof(char *) * numWords);
    allWords[numWords - 1] = malloc((sizeof(char) * strlen(word)) + 1);
    strlcpy(allWords[numWords - 1], word, strlen(word));
    word = strtok(NULL, " ");
  }
  *count = numWords;
  return allWords;
}
