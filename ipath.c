#include <bsd/stdlib.h>
#include <stdio.h>
#include <zconf.h>

/**
 * Generate and return an integer array which is a random permutation of the
 * values from low to high. The return array is dynamically allocated and should
 * be free'd by the caller.
 *
 * @param low Lower bound
 * @param high Upper bound
 * @param length Will be set by reference by this function, can be used by the
 * caller to determine the length of the returned array
 *
 * @return Returns an integer array which is a random permutation of the values
 * from low to high. On error will return a NULL pointer and length will be set
 * to -1.
 */
int *getIPathArray(int low, int high, int *length) {
  *length = -1;  // Will contain length of the permutation array once that value
                 // is calculated
  int *values = NULL;  // Pointer to integer array which will hold random
                       // permutation of low to high

  // Validate
  if (low < 0 || high < 0 || low > high) {
    printf(
        "ERROR: Both low and high must be 0 or greater and low may not be "
        "greater than high.");
    return NULL;
  }

  // Ensure dont have 0 AND INT_MAX
  if (low == 0 && high == INT_MAX) {
    printf("ERROR: Low as 0 and High as INT_MAX exceeds maximum length.\n");
    return NULL;
  }

  // Collect length
  *length = high - low + 1;

  // Allocate memory for the randomly permuted array
  values = calloc(*length, sizeof(int));

  // Verify calloc call
  if (values == NULL) {
    printf("ERROR: There was an error allocating memory for i path array.\n");
    return NULL;
  }

  // Init all values in values array to a sentinel of -1
  for (int i = 0; i < *length; i++) {
    values[i] = -1;
  }

  // For each number from low to high
  for (int i = low; i <= high; i++) {
    int randomIndex = -1;
    int openIndexFound = 0;

    do {
      // Get a random index
      randomIndex = arc4random_uniform(*length);

      // If the index is unused, then insert this low-high value at the open
      // index
      if (values[randomIndex] == -1) {
        values[randomIndex] = i;
        openIndexFound = 1;
      }

      // If the random index was already occupied then keep trying until we find
      // an open one
    } while (!openIndexFound);
  }

  return values;
}