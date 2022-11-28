#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
void limit_fork(rlim_t max_procs)
{
    struct rlimit rl;
    if (getrlimit(RLIMIT_NPROC, &rl))
    {
        perror("getrlimit");
        exit(-1);
    }
    rl.rlim_cur = max_procs;
    if (setrlimit(RLIMIT_NPROC, &rl))
    {
        perror("setrlimit");
        exit(-1);
    }
}



int create_curl_process(char* filename, char *timelim_string, char *urlstring, ssize_t *childcount, ssize_t linecounter){
    int timelim = atoi(timelim_string);
    pid_t child;
    if ((child = fork()) < 0){
        // error
        fprintf(stderr, "fork of process to handle download from %s failed\n", urlstring);
        exit(1);
    } else if (child == 0){
        // child
        int fd;
        if ((fd = open(filename, O_WRONLY | O_CREAT, 644)) < 0){
            // if unable to open download file, process can't continue, kill the child
            printf("Error from process handling download from %s, unable to open download file %s\n", urlstring, filename);
            exit(0);
        } else{
            // exec to curl
            if (timelim > 0){
                execlp("curl", "curl",  "-m", timelim_string, "-o", filename, "-s", urlstring,  NULL);
            } else{
                execlp("curl", "curl", "-o", filename, "-s", urlstring,  NULL);
            }   
            printf("Error from process handilng download from %s, exec failed\n", urlstring);
            return 2;
        }
        exit(0); // if exec doesn't go through kill the child 
    } else {
        // parent
        printf("process %d processing line #%zd\n", child, linecounter);
        (*childcount)++;
        return 0;
    }
}

int wait_for_child(ssize_t *childcount){
    int status;
    pid_t pid = wait(&status);

    if(WIFEXITED(status)){  
        printf("process %d completed \n", pid);

        if(WEXITSTATUS(status) != 0){
            // curl encountered an error
            printf("curl process with pid %d encountered an error\n", pid);
        } else {
            (*childcount) --;
            // create new process for curling next url
        }
    } else if (pid != -1) {
        // uh-oh
        // bad exit from child
        printf("bad exit from process with pid %d\n", pid);
        (*childcount)--;
        return -1;
    } else {
        (*childcount)--;
    }
    return 0;
}



int main(int argc, char *argv[]) {
    
    limit_fork(400);
    // check for correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: ./a.out <filename> <child process limit>\n");
        exit(1);
    }

    // open file for reading
    FILE *fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }

    // read the child process limit from command line args
    ssize_t childlimit = atoi(argv[2]);
    if(childlimit < 1){
        fprintf(stderr, "Error, child process limit not valid\n");
        exit(1);
    }

    // read lines from file
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    ssize_t childcount = 0;
    ssize_t linecounter = 0;
    while(((nread = getline(&line, &len, fptr)) != -1) || childcount > 0){
        if (nread > 0){
            linecounter ++;
            // extract filename, url string, and integer from line
            char *filename = strsep(&line, " |\n|\t");
            char *urlstring = strsep(&line, " |\n|\t");
            char *timelim_string = NULL; // default to NULL if no time limit given check timelim later and curl with no limit for 0 case.

            // instantiate timelim and timelim_string if an int is given
            if (line != NULL) {
                timelim_string = strsep(&line, " |\n|\t");
            }

            // check that filename is valid
            if (filename == NULL || strlen(filename) == 0) {
                fprintf(stderr, "Invalid filename\n");
                continue;
            }

            // check that timelim is valid
            if (atoi(timelim_string) < 0) {
                fprintf(stderr, "Invalid integer %s\n", timelim_string);
                continue;
            }
            // fork new process or if limit reached, wait for next process to finish then fork a new one.
            if (childcount >= childlimit){
                // child limit reached 
                // wait for next child to return then fork a new process after cleanup
                wait_for_child(&childcount);
                // child returned and exit status handled, create next process
                create_curl_process(filename, timelim_string, urlstring, &childcount, linecounter);
            } else {
                // child limit not yet reached
                // fork a new process for next curl download
                create_curl_process(filename, timelim_string, urlstring, &childcount, linecounter);
            }
        // }else {
        //     if(childcount >0){
        //         wait_for_child(&childcount);
        //     }
        // }
        }else{
            if(childcount > 0){
                wait_for_child(&childcount);
            }
        }
        // if(line != NULL){
        //     free(line);
        // }
    }
    // cleanup 
    // close files
    fclose(fptr);
    // free memory
    
    return 0;
    
}
