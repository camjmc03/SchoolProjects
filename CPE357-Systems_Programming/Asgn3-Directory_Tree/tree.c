#define _POSIX_SOURCE
#include <unistd.h>
#undef _POSIX_SOURCE
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include <errno.h>

void print_dir_tree_inner(int level, char *dir_name, int a_flag, int s_flag){
    if (chdir(dir_name)){
        printf("attempt to access directory %s was unsuccessful\n", dir_name);
    } else {
        DIR *dp = NULL;
        struct dirent *ep = NULL;
        struct stat estat;

        dp = opendir(".");

        char **entries;
        ssize_t i=0;
        size_t entrycount=0;
        
        if (dp != NULL){
            // create array of char pointers to hold all subdir names
            while ((ep = readdir(dp)) != NULL) {
                    entrycount++;
            }

            // store all subdirs names in the array
            entries = malloc(sizeof(char*) * entrycount);
            rewinddir(dp);
            while ((ep = readdir(dp)) != NULL) {
                entries[i] = malloc(sizeof(char) * (strlen(ep->d_name) + 1));
                strcpy(entries[i], ep->d_name);
                i++; 
            }

            // close dp so file descriptor resources are preserved
            closedir(dp);

            // sort the entries for the current directory
            qsort(entries, entrycount, sizeof(char *), compare_strings);
            // loop through the array of subdir names and do print logic if necessary 
            for (i = 0; i < entrycount; i++){
                
                if (!a_flag && entries[i][0] == '.'){
                    continue;
                }

                if(stat(entries[i], &estat) == 0){
                    if (S_ISDIR(estat.st_mode)){
                        
                        if (strcmp(entries[i], ".") == 0 ||
                        strcmp(entries[i], "..") == 0){
                            continue;
                        }
                        
                        for (int i = 0; i < level; i++){
                            printf("|    ");
                        }
                        if(s_flag == 1){
                            printf("|-- %s [size: %d]\n", entries[i], (int) estat.st_size);
                        } else{
                            printf("|-- %s\n", entries[i]);
                        }
                        print_dir_tree_inner(level + 1, entries[i], a_flag, s_flag);
                        if(chdir("..")){
                            printf("unable to chdir back into parent file %s\n",dir_name);
                        }
                    } else{
                        for (int i = 0; i < level; i++){
                            printf("|    ");
                        }
                        if (s_flag == 1){
                            printf("|-- %s [size: %d]\n", entries[i], (int) estat.st_size);
                        }else{
                            printf("|-- %s\n", entries[i]);
                        }
                    }
                }else {
                    printf("could not call stat for entry %s, errno=%d\n", entries[i], errno);
                }
                
            }
            
            // cleanup
            for(i = 0; i < entrycount; i++) {
                free(entries[i]);
            }
            free(entries);
        } else{
            perror("Couldn't open directory");
            exit(1);
        }
    }
}

void print_dir_tree(char *dir_name, int a_flag, int s_flag){
    printf("%s:\n",dir_name);
    print_dir_tree_inner(0, dir_name, a_flag, s_flag);
    printf("\n");
}

int main(int argc, char **argv){
    char dir_name[1024];
    int a_flag = 0;             // flag for inclusion of hidden files
    int s_flag = 0;             // flag for inclusion of file sizes
    for(int i = 1; i < argc; i++){
        if(setifflag(argv[i], &a_flag, &s_flag)){
            strcpy(dir_name,argv[i]); 
            print_dir_tree(dir_name, a_flag, s_flag);
            chdir("..");
        }
    }
    return 0;
}
