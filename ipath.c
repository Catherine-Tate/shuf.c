#include <stdio.h>
#include <zconf.h>
#include <bsd/stdlib.h>

int main(int argc, char **argv)
{
    int i = 0;                      // Loop variable
    int low = 0;                    // Low end of range
    int high = 0;                   // High end of range
    int range = 0;                  // Total length of range
    int * values = NULL;            // Pointer to integer array which will hold random permutation of low to high

    // Verify number of command line arguments
    if (argc != 4)
    {
        return EXIT_FAILURE;
    }

    // Collect low and high as integers
    low = (int)strtol(argv[2], NULL, 10);
    high = (int)strtol(argv[3], NULL, 10);

    // Validate
    if(low < 0 || high < 0 || low > high)
    {
        printf("ERROR: Both low and high must be 0 or greater and low may not be greater than high.");
        exit(EXIT_FAILURE);
    }

    // Ensure dont have 0 AND INT_MAX
    if(low == 0 && high == INT_MAX)
    {
        printf("ERROR: Low as 0 and High as INT_MAX exceeds maximum range.\n");
        exit(EXIT_FAILURE);
    }

    printf("INT_MAX is %d\n", INT_MAX);

    // Collect range
    range = high - low + 1;

    printf("Low was %d\n", low);
    printf("High was %d\n", high);
    printf("Range is %d\n", range);

    // Allocate memory for the randomly permuted array
    values = calloc(range, sizeof(int));

    // Init all values in values array to a sentinel of -1
    for(i = 0; i < range; i++)
    {
        values[i] = -1;
    }

    // For each number from low to high
    for(i = low; i <= high; i++)
    {
        int index = -1;
        int openIndexFound = 0;

        do
        {
            // Get a random index
            index = arc4random_uniform(range);

            // If the index is unused, then insert this low-high value at the open index
            if(values[index] == -1)
            {
                values[index] = i;
                openIndexFound = 1;
            }

            // If the random index was already occupied then keep trying until we find an open one
        } while (!openIndexFound);
    }

    for(i = 0; i < range; i++)
    {
        printf("%d\n", values[i]);
    }

    return 0;
}
