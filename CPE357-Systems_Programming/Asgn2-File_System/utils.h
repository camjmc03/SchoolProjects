#ifndef UTILS_H 
#define UTILS_H
#endif

char *uint32_to_str(uint32_t i);
void validate_args(int argc, char *argv[]);
void init_inodes(char *inode_array);
void tokenize_command(char *in, char **command, char **commandarg);
char *checked_malloc(int size);
