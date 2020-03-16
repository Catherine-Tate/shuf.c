#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <limits.h>

#define PROGRAM_NAME "SHUF 1.0"
#define AUTHORS "BLUE TEAM"
#define DATE "July 13, 2017"
#define VERSION "Debian"







int readInput(int argc, char**argv, char *** lines)
{
    FILE * fp;
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;

    if(argc == 0 || (argv[0][0] == '-' && argv[0][1] == '\0'))
    {
     fp = stdin;

    }
    else
    {        
        fp = fopen(argv[0], "r");
    }
    while ((line_size = getline(&line_buf, &line_buf_size, fp)) != -1) {
        *lines[line_count] = line_buf;
        line_count ++;
        line_buf = NULL;
        line_buf_size = 0;
    }
    return line_count;
}


int main(int argc, char **argv)
{

    char ** arguments = NULL;



    int eflag = 0; // Use a space seperated list of command line args as input
    int iflag = 0; // Use range of positive integers from lo to hi as input (Cannot be used with eflag)
    int hflag = 0; // Print usage information and exit
    int rflag = 0; // Do not permute. Instead, choose line at random, with replacement. When used without -n, shuf repeats indefinitely.
    int vflag = 0; // Print version information and exit
    int zflag = 0; // Delineate lines with NUL character instead of the newline character
    
    char *loHi = NULL; // Array of our low high value
    char *count = NULL; // Count of how many lines we will want to print
    char const *outfile = NULL; // Outfile if provided
    
    int index;
    int c;

    opterr = 0;

    while ((c = getopt(argc, argv, "hvinorz:")) != -1)
        switch (c)
        {
        

        case 'e':
            eflag = 1;
            break;


        case 'i':
            iflag = 1;
            loHi = optarg;
            break;


        case 'n':
            count = optarg;
            break;
        
        case 'z':
            zflag = 1;
            break;
        
        case 'r':
            rflag = 1;
            break;
        
        case 'o':
            if(outfile != NULL)
                printf("Error Multiple Output Files Specified.");
                return EXIT_FAILURE;
            outfile = optarg;
            break;

        case 'v':
            printf("Program Name: %s\nVersion: %s\nDate : %s\nAuthors: %s\n", PROGRAM_NAME, VERSION, DATE, AUTHORS);
            return EXIT_SUCCESS;

        case 'h':
            printf("SYNOPSIS\nshuf [-hv] [-n count] [-o outfile] [-rz] [file]\nshuf [-hv] -e [-n count] [-o outfile] [-rz] [args ...]\nshuf [-hv] -i lo-hi [-n count] [-o outfile] [-rz]\n");
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
    if(eflag)
    {
        if(iflag)
        {
            return EXIT_FAILURE; // eflag and iflag cant be used together
        }


        /*code*/

    }
    // HANDLE THE CASE OF I FLAG
    else if (iflag)
    {
        /*code*/
    }
    // HANDLE THE CASE OF NO FLAG
    else
    {   
        char ** lines = NULL;
        int numLines = readInput(argc, argv, &lines);
    }
    

    

    printf("lohi = %s\n",loHi);
    printf("count = %s\n",count);


    for (index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
    return 0;

    return EXIT_SUCCESS;
}