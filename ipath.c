#include <bsd/stdlib.h>
#include <math.h>
#include <stdio.h>
#include <zconf.h>

#include "util.h"

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
char **getIPathArray(int low, int high, int *length) {
  *length = -1;  // Will contain length of the permutation array once that value
                 // is calculated
  char **values = NULL;  // Pointer to integer array which will hold random
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
  values = (char **)calloc(*length, sizeof(char *));

  // Verify calloc call
  if (values == NULL) {
    printf("ERROR: There was an error allocating memory for i path array.\n");
    return NULL;
  }

  // the number of digits in INT_MAX (and therefore all valid numbers)
  int max_digits = (int)ceil(log10((double)INT_MAX));

  // initialize array with numbers in range
  int written = 0;
  for (int i = 0; i < *length; i++) {
    values[i] = (char *)calloc(max_digits, sizeof(char));
    written = snprintf(values[i], max_digits, "%d", low + i);
    if (written <= 0) {
      perror("ERROR: Unable to initialize numbern.\n");
    }
  }

  shuffle(values, *length);
  return values;
}
