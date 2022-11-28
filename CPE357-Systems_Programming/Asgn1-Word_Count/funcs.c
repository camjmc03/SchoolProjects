#define NUM_ARGS 2
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

int validate_args (int argc, char *argv[]){
    if(argc == 2){
        return 1;
    } else if(argc == 1){
        return 0;
    }else {
        fprintf(stderr, "usage: %s file, %s", argv[0], argv[0]);
        exit(1);
    }
}

int validate_fp(FILE *file){
    if (file) {
        return 0;
    } else{
        return 1;
    }
}
