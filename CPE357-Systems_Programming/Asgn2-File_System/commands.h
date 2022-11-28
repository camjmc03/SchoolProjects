#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "utils.h"
#include <sys/stat.h>

void c_exit();
void c_cd(char *inp, uint32_t *current_directory, char *inode_array);
void c_ls(uint32_t *current_directory);
void c_mkdir(char* inp, uint32_t *current_directory, char *inode_array);
void c_touch(char* inp, uint32_t *current_directory, char *inode_array);
