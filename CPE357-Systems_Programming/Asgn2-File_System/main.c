#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> 
#include <unistd.h>
#include "utils.h"
#include "commands.h"

int main(int argc, char * argv[]){
    uint32_t current_directory = 0; // current directory stored as int for easy indexing of inodes

    // inode array of size 1024
    char inode_array[1024];
    // initialize the contents of the array based on the given filesystem in argv[1]
    validate_args(argc, argv);
    chdir(argv[1]);
    init_inodes(&(inode_array[0]));

    // get command from user in loop
    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, stdin)) != -1){
        // valid commands: exit, cd, ls, mkdir, touch
        // holders for command and the value passed with it
        char *command = NULL;
        char *commandarg = NULL;
        // validate command takes in line and pointer to two char arrays, puts tokens into char arrays if they form a valid command.
        tokenize_command(line, &command, &commandarg);
        if (command != NULL){
            // call the commands.c func based on what was returned in vc
            if(!strcmp(command, "\0")){
                c_exit();
            }else if(!strcmp(command, "exit")){
                c_exit();
            } else if (!strcmp(command, "cd")){
                c_cd(commandarg, &current_directory, &(inode_array[0]));
                // printf("%d\n", current_directory);
            } else if (!strcmp(command, "ls")){
                c_ls(&current_directory);
            } else if (!strcmp(command, "mkdir")){
                c_mkdir(commandarg, &current_directory, &(inode_array[0]));
            } else if (!strcmp(command, "touch")){
                c_touch(commandarg, &current_directory, &(inode_array[0]));
            } else {
                printf("unknown command %s \n", command);
            }
        }
    }
    // on exit, update the inodes_list file to reflect changes made
}
