#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *checked_malloc(int size){
    char *p = malloc(sizeof(char) * size);
    if (p== NULL){
        perror("malloc of char * didn't work as expected");
        exit(1);
    }
    return p;
}

char *uint32_to_str(uint32_t i){
    int length = snprintf(NULL, 0, "%lu", (unsigned long)i);       // pretend to print to a string to get length
    char* str = checked_malloc(length + 1);                        // allocate space for the actual string
    snprintf(str, length + 1, "%lu", (unsigned long)i);           // print to string
    return str;
}

void validate_args(int argc, char *argv[]){
    if (argc !=2){
        fprintf(stderr, "usage: %s file, %s", argv[0], argv[0]);
        exit(1);
    }
}

void init_inodes(char *inode_array){
    FILE *inode_list;
    inode_list = fopen("inodes_list", "r");

    if (inode_list == NULL){
        fprintf(stderr, "Error: Could not open inodes_list for given directory or given directory does not exits\n");
    }

    int read;
    uint32_t curindex = 0;

    // read in inodes in a loop until end of file reached
    while ((read = fread(&curindex, sizeof(uint32_t), 1, inode_list)) != 0){
        // read first 32 bits as a uint32_t for the index
        
        // index is out of bounds report it and skip it
        if(curindex > 1023){
            printf("index %d read from inodes_list is out of bounds\n", curindex);
        } else{
            // read second part of entry as a char
            // if the filetype is invalid, skip it and move on
            char curfiletype;
            read = fread(&curfiletype, sizeof(char), 1, inode_list);
            if(curfiletype == 'f' | curfiletype == 'd' | curfiletype == '\0'){
                inode_array[curindex] = curfiletype;
            } else {
                printf("invalid filetype at index %d in inodes_list\n", curindex);
            }
        } 
    }
    fclose(inode_list);
}

void tokenize_command(char *in, char **command, char **commandarg){
    // use strsep to seperate into command and argument. 
    if (in[0] != '\n'){
        *command = strsep(&in, " \n");
        *commandarg = strsep(&in, " \n");
    } else {
        *command = " ";
        *commandarg = " ";
    }
    
}


