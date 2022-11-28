#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "funcs.h"


void unique(FILE *in){
    //takes in a file stream and prints to console each line that is not preceded by its duplicate
    char *line = "";
    size_t len = 0;
    size_t read;
    char *prevLine = "";
    while ((read = getline(&line, &len, in)) != -1) {
        // if the current line is not the same as the previous line
        if (strcmp(prevLine, line) != 0) {
            printf("%s", line);
            prevLine = line;
            line = NULL; // causes getline to make a new buffer next time its called
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");
    int vacode = validate_args(argc, argv);
    if (vacode == 1){ // if file specified
        if (validate_fp(file) == 1){ // check validity of the file
            fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
            return 1;
        }else{
            unique(file);
            fclose(file);
        }
    } else if (vacode == 0){ // if no file specified read from stdin
        unique(stdin);
    }
    return 0;
}
