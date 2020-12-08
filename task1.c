#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

void new_stdin(char *s) {
    int f = open(s, O_RDONLY);
    if(f == -1) perror("");
    close(0);
    dup2(f, 0);
    close(f);
}

void conv_out(int fd) {
    close(1);
    dup2(fd, 1);
    close(fd);
}

void conv_in(int fd) {
    close(0);
    dup2(fd, 0);
    close(fd);
}

int main(int argc, char** argv) {
    int fd[2];
    int i;
    int error = 1;
    pipe(fd);

    if(!fork()) {
        new_stdin(argv[4]);
        conv_out(fd[1]);
        execlp(argv[1], argv[1],  NULL);
        exit(1);
    } else wait(0);

    if(!fork()) {
        conv_in(fd[0]);
        execlp(argv[2], argv[2], NULL);
        error = 0;
        exit(1);
    } else wait(0);
    
    if(error) {
        if(!fork()) {
            conv_in(fd[0]);
            execlp(argv[3], argv[3], NULL);
            exit(1);
        } else wait(0);
    }
}
