#include "funcs.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NUM_ARGS 2
#define FILE_IDX 1
#define OPEN_MODE "r"

void word_count(FILE *in, int *counts) {
    size_t c;
    size_t prevc = '\0';
    if(in == NULL){ // if no file specified read from stdin
        in = stdin;
    }
    while ((c = fgetc(in)) != EOF) {
        counts[0]++;       // add to the char count if there is a next char
        if ((isspace(c)) && !isspace(prevc)) {
            counts[1]++;   // add to the word count if c is any whitespace char
        }
        prevc = c;
        if ( c == '\n'){
            counts[2]++;   // add to the line count if c is a newline char
        }
    }
    //while loop doesn't catch last line
    if (counts[0] > 0){
        counts[2]++;
        if (!isspace(prevc)){
            counts[1]++;
        }
    }
}

int main(int argc, char *argv[]){
    int vacode = validate_args(argc, argv);
    // char, word, and line count in an array so references can all be passed at once and manipulated by other funcs
    int out[3];
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
    // check if validate_args determined if there was a file or if input should come from stdin
    if (vacode == 0){ // stdin input
        word_count(NULL, out);
        printf("char count: %d\nword count: %d\nline count: %d", out[0], out[1], out[2]);
    } else if (vacode == 1){ // file input
        FILE *in;
        in = fopen(argv[FILE_IDX], OPEN_MODE);
        
        word_count(in, out);
        printf("\nchar count: %d\nword count: %d\nline count: %d", out[0], out[1], out[2]);
        fclose(in);
    }
    return 0;
}
