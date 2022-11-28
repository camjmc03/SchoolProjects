#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "utils.h"
#include <sys/stat.h>

void c_exit(){
    // goodbye
    exit(0);
}
void c_cd(char *inp, uint32_t *current_directory, char *inode_array){

    // open current directory
    char * currentdirectory_str;
    currentdirectory_str = uint32_to_str(*current_directory);
    FILE* curdir_fp = fopen(currentdirectory_str, "r");

    // read through inode by inode in the curent directory
    // and check if the name associated with the inode matches inputted directory
    uint32_t curindex = -1;
    char curname[33];
    curname[32] = '\0';
    int read;
    int changeflag = 0;
    while ((read = fread(&curindex, sizeof(uint32_t), 1, curdir_fp)) > 0){ 
        if (curindex < 1024){
            read = fread(&curname, sizeof(char), 32, curdir_fp);
            // if the name of the current directory matches and is a valid directory
            // change current directory to that index
            if (!strcmp(&(curname[0]), inp)){
                if (inode_array[curindex] == 'd'){
                    *current_directory = curindex;
                    changeflag = 1;
                    break;
                } else {
                    printf ("%s is not a directory\n", inp);
                }
            }
        } else {
            printf("ERROR: inode index %d out of bounds for directory, %s\n", curindex, inp);
        }
    }
    if(!changeflag){
        printf(" could not find directory %s\n", inp);
    }
    fclose(curdir_fp);
    free(currentdirectory_str);
}
void c_ls(uint32_t* current_directory){
    // open current directory
    char * curdirectory_str;
    curdirectory_str = uint32_to_str(*current_directory);
    // sprintf(curdirectory_str, "%d", *current_directory);
    FILE* curdir_fp = fopen(curdirectory_str, "r");

    // read through inode by inode in the curent directory
    // and print to stdout
    uint32_t curindex = -1;
    char curname[33];
    curname[32] = '\0';
    int read;
    while ((read = fread(&curindex, sizeof(uint32_t), 1, curdir_fp)) > 0){ 
       
        read = fread(&curname, sizeof(char), 32, curdir_fp);
        printf("%d: %s\n", curindex, curname);
    }
    free(curdirectory_str);
    fclose(curdir_fp);
    
}

void c_mkdir(char* inp, uint32_t *current_directory, char *inode_array){
    // open current directory for checking if the file already exists
    char *curdirectory_str;
    curdirectory_str = uint32_to_str(*current_directory);
    FILE* curdir_read_fp = fopen(curdirectory_str, "r");
    // read through inode by inode in the curent directory
    // and check if the name associated with the inode matches inputted directory
    uint32_t curindex = -1; //-1 = highest uint value this will trip if statement later if error
    char curname[33];
    curname[32] = '\0';
    int read;
    int makeflag = 1;
    while ((read = fread(&curindex, sizeof(uint32_t), 1, curdir_read_fp)) > 0){ 
        if (curindex < 1024){
            read = fread(&curname, sizeof(char), 32, curdir_read_fp);
            // if the name of the current directory matches and is a valid directory
            // change current directory to that index
            if (!strcmp(&(curname[0]), inp)){
                makeflag = 0;
            }
        } else {
            printf("ERROR: inode index %d out of bounds for directory, %s\n", curindex, inp);
        }
    }
    fclose(curdir_read_fp);
    if(makeflag){
        
        // find first unused inode_array index
        uint32_t curinode = 0;

        while(inode_array[curinode] != '\0' && curinode < 1024){
            curinode++;
        }
        // curinode is now the first unused inode index
        // check that it is not out of bounds
        if(curinode < 1024){
            // switch that index's value to 'd' 
            inode_array[curinode] = 'd';

            // write to curdir_append_fp the new directory's index and name
            FILE* curdir_append_fp = fopen(curdirectory_str, "a");
            fwrite(&curinode, sizeof(uint32_t), 1, curdir_append_fp);
            // create new 32 length array of chars
            int i = 0;
            // write characters 
            while(inp[i] != '\0' && i < 32){
                fwrite(&inp[i], sizeof(char), 1, curdir_append_fp);
                i++;
            }
            while(i < 33){
                fwrite("\0", sizeof(char), 1, curdir_append_fp);
                i++;
            }
            fclose(curdir_append_fp);

            // write to inodes_list the new filetype for the new inode
            // open inodes_list
            FILE * inodes_list = fopen("inodes_list", "a");
            // write new inode with type 'd'
            fwrite(&curindex, sizeof(uint32_t), 1, inodes_list);
            fwrite("d", sizeof(char), 1, inodes_list);

            // create new directory file in the filesystem with the inode as its name
            // write to the directory file "0.0.."
            char * curinode_str = uint32_to_str(curindex);
            uint32_t zerouint32 = 0;

            FILE *new_dir = fopen(curinode_str, "w");
            fwrite(&zerouint32, sizeof(uint32_t), 1, new_dir);
            fwrite(".", sizeof(char), 1, new_dir);
            fwrite("\0", sizeof(char), 31, new_dir);
            fwrite(&zerouint32, sizeof(uint32_t), 1, new_dir);
            fwrite("..", sizeof(char), 2, new_dir);
            fwrite("\0", sizeof(char), 30, new_dir);

            //cleanup
            fclose(new_dir);
            fclose(inodes_list);
            free(curinode_str);

        } else {
            printf("maximum number of files for this filesystem reached\n");
        }
        
    } else {
        printf("%s already exists in this directory\n", curdirectory_str);
    }
    free(curdirectory_str);
}

void c_touch(char* inp, uint32_t *current_directory, char *inode_array){
    // open current directory for checking if the file already exists
    char *curdirectory_str;
    curdirectory_str = uint32_to_str(*current_directory);
    FILE* curdir_read_fp = fopen(curdirectory_str, "r");
    // read through inode by inode in the curent directory
    // and check if the name associated with the inode matches inputted directory
    uint32_t curindex = -1; // initialize current index highest uint value so unless something is read under 1024 it trips if statement
    char curname[33];
    curname[32] = '\0';
    int read;
    int makeflag = 1;
    while ((read = fread(&curindex, sizeof(uint32_t), 1, curdir_read_fp)) > 0){ 
        if (curindex < 1024){
            read = fread(&curname, sizeof(char), 32, curdir_read_fp);
            // if the name of the current directory matches and is a valid directory
            // change current directory to that index
            if (!strcmp(&(curname[0]), inp)){
                makeflag = 0;
            }
        } else {
            printf("ERROR: inode index %d out of bounds for directory, %s\n", curindex, inp);
        }
    }
    fclose(curdir_read_fp);
    if(makeflag){
        
        // find first unused inode_array index
        uint32_t curinode = 0;

        while(inode_array[curinode] != '\0' && curinode < 1024){
            curinode++;
        }
        // curinode is now the first unused inode index
        // check that it is not out of bounds
        if(curinode < 1024){
            // switch that index's value to 'f' 
            inode_array[curinode] = 'f';

            // write to curdir_append_fp the new directory's index and name
            FILE* curdir_append_fp = fopen(curdirectory_str, "a");
            fwrite(&curindex, sizeof(uint32_t), 1, curdir_append_fp);
            // create new 32 length array of chars
            int i = 0;
            // write characters 
            while(inp[i] != '\0' && i < 32){
                fwrite(&inp[i], sizeof(char), 1, curdir_append_fp);
                i++;
            }
            while(i < 33){
                fwrite("\0", sizeof(char), 1, curdir_append_fp);
                i++;
            }
            fclose(curdir_append_fp);

            // write to inodes_list the new filetype for the new inode
            // open inodes_list
            FILE * inodes_list = fopen("inodes_list", "a");
            // write new inode with type 'f'
            fwrite(&curindex, sizeof(uint32_t), 1, inodes_list);
            fwrite("f", sizeof(char), 1, inodes_list);

            //cleanup
            fclose(inodes_list);


        } else {
            printf("maximum number of files for this filesystem reached\n");
        }
        
    } else {
        printf("%s already exists in this directory\n", curdirectory_str);
    }
    free(curdirectory_str);
}
