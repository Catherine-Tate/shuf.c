#include <bsd/stdlib.h>
#include <bsd/string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ipath.h"

#define PROGRAM_NAME "SHUF 1.0"
#define AUTHORS "BLUE TEAM"
#define DATE "July 13, 2017"
#define VERSION "Debian"

// takes input arguments (either from file, -e, or -i) and prints randomly based
// on options
void printRandomLine(char **inputLines, int numLines, int z, int r,
                     long count) {
  int removed = 0;
  int random = -1;
  while (count != 0) {
    random = arc4random_uniform(numLines);
    if (strcmp(inputLines[random], "") == 0) continue;
    printf("%s", inputLines[random]);
    if (!z) printf("\n");
    if (!r) {
      inputLines[random] = "";
      removed++;
    }
    if (removed == numLines) break;
    count--;
  }
}

int readInput(int argc, char **argv, char ***lines) {
  FILE *fp;
  char *line_buf = NULL;
  size_t line_buf_size = 0;
  int line_count = 0;
  ssize_t line_size;

  lines = calloc(1, sizeof(char *));

  if (argc == 0 || (argv[0][0] == '-' && argv[0][1] == '\0')) {
    fp = stdin;

  } else {
    fp = fopen(argv[0], "r");
  }
  while ((line_size = getline(&line_buf, &line_buf_size, fp)) != -1) {
    lines = realloc(lines, line_count * sizeof(char *));
    *lines[line_count] = line_buf;
    line_count++;
    line_buf = NULL;
    line_buf_size = 0;
  }
  return line_count;
}

int main(int argc, char **argv) {
  // char ** arguments = NULL;

  int eflag = 0;  // Use a space seperated list of command line args as input
  int iflag = 0;  // Use range of positive integers from lo to hi as input
                  // (Cannot be used with eflag)
  int hflag = 0;  // Print usage information and exit
  int rflag =
      0;  // Do not permute. Instead, choose line at random, with replacement.
          // When used without -n, shuf repeats indefinitely.
  int vflag = 0;  // Print version information and exit
  int zflag =
      0;  // Delineate lines with NUL character instead of the newline character

  char *loHi = NULL;           // Array of our low high value
  long count = -1;             // Count of how many lines we will want to print
  char **eArgs = NULL;         // Arguments provided for e option
  FILE *outfile = NULL;  // Outfile if provided
  int numWords = 0;

  int index;
  int c;

  opterr = 0;

  while ((c = getopt(argc, argv, "hvi:n:o:rze:")) != -1) switch (c) {
      case 'e':
        eflag = 1;
        index = optind - 1;
        // printf("%s\n", optarg)
        eArgs = malloc(sizeof(char *));
        while (index < argc) {
          // printf("%s\n", argv[index]);
          if (argv[index][0] != '-') {
            eArgs[numWords] = malloc((strlen(argv[index]) + 1) * sizeof(char));
            strlcpy(eArgs[numWords], argv[index], strlen(argv[index]) + 1);
            numWords++;
            eArgs = realloc(eArgs, sizeof(char *) * (numWords + 1));
          } else
            break;
          index++;
        }
        // malloc((sizeof(optarg)*sizeof(char*))+1);
        optind = index - 1;
        break;

      case 'i':
        iflag = 1;
        loHi = optarg;
        break;

      case 'n':
        errno = 0;
        char *errCatch;
        count = strtol(optarg, &errCatch, 10);
        if (strcmp(errCatch, optarg) == 0) {
          fprintf(stderr, "Invalid Line Count: \'%s\'\n", errCatch);
        }
        // printf("%lu\n%s\n", count, errCatch);
        break;

      case 'z':
        zflag = 1;
        break;

      case 'r':
        rflag = 1;
        break;

      case 'o':
        if (outfile != NULL) {
          printf("Error Multiple Output Files Specified.");
          return EXIT_FAILURE;
        }
        outfile = fopen(optarg, "w");
        break;

      case 'v':
        printf("Program Name: %s\nVersion: %s\nDate : %s\nAuthors: %s\n",
               PROGRAM_NAME, VERSION, DATE, AUTHORS);
        return EXIT_SUCCESS;

      case 'h':
        printf(
            "SYNOPSIS\nshuf [-hv] [-n count] [-o outfile] [-rz] [file]\nshuf "
            "[-hv] -e [-n count] [-o outfile] [-rz] [args ...]\nshuf [-hv] -i "
            "lo-hi [-n count] [-o outfile] [-rz]\n");
        return EXIT_SUCCESS;

      case '?':
        if (optopt == 'i' || optopt == 'n')
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint(optopt))
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        abort();
    }

  argc -= optind;
  argv += optind;

  // HANDLE THE CASE OF E FLAG
  if (eflag) {
    if (iflag) {
      return EXIT_FAILURE;  // eflag and iflag cant be used together
    }

    /*code*/
    printRandomLine(eArgs, numWords, zflag, rflag, count);
  }
  // HANDLE THE CASE OF I FLAG
  else if (iflag) {
    char * nums = malloc((strlen(loHi) * sizeof(char)) + 1);
    strlcpy(nums, loHi, sizeof(loHi));
    nums = strtok(nums, "-");
    int low = (int)strtol(nums, NULL, 10);         // Low end of range
    nums = strtok(NULL, "-");
    int high = (int)strtol(nums, NULL, 10);        // High end of range
    int length = 0;      // The length of the random permutation array
    int *values = NULL;  // Random permutation array

    // Collect low and high as integers
    //low = (int)strtol(argv[2], NULL, 10);
    //high = (int)strtol(argv[3], NULL, 10);

    // Get the permuted array
    values = getIPathArray(low, high, &length);

    // Check for errors
    if (values == NULL) {
      return EXIT_FAILURE;
    }

    // Test Print
    for (int i = 0; i < length; i++) {
      printf("%d\n", values[i]);
    }

  }
  // HANDLE THE CASE OF NO FLAG
  else {
    char **lines = NULL;
    int numLines = readInput(argc, argv, &lines);
    printRandomLine(lines, numLines, zflag, rflag, numLines);
  }

  // printf("lohi = %s\n",loHi);
  // printf("count = %s\n",count);

  for (index = optind; index < argc; index++)
    printf("Non-option argument %s\n", argv[index]);
  return 0;

  return EXIT_SUCCESS;
}
