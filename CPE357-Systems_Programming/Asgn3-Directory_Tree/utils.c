#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int setifflag(char* arg, int *a_flag, int *s_flag){
    /*  
    sets a or s flag if the passed in char * is  "-a" or "-s" and returns 0
    returns -1 if the char * is not a flag
    */
        if (!strcmp(arg, "-a") && !*a_flag){
            *a_flag = 1;
            return 0;
        } else if (!strcmp(arg, "-s") && !*s_flag){
            *s_flag = 1;
            return 0;
        }
        return -1;
}

int compare_strings(const void *a, const void *b)
{
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
}
