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

int strtonum(char* s){
    int n = 0;
    int i = 0;
    while(s[i] != '\0') {
        n = n * 10 + (s[i] - '0');
        i++;
    }
    return n;
}

char* numtostr(int n){
    char s[40], t, *temp = NULL;
    int i, k;
    i = 0;
    k = n;
    do {
        t = k % 10;
        k = k / 10;
        s[i] = t | 0x30;
        i++;
    } while (k>0);
    s[i] = '\0';
    temp = malloc(strlen(s) + 1);
    k = 0;
    i--;
    while (i >= 0) {
        temp[k] = s[i];
        i--;
        k++;
    }
    temp[k] = '\0';
    return temp;
}

int main(int argc, char** argv){
    int n = strtonum(argv[1]);
    int i;
    char** mas = malloc(sizeof(char*) * (n + 2));
    mas[0] = malloc(sizeof(char) * 5);
    strcpy(mas[0],"echo\0");
    for(i = 1; i <= n; i++) {
        char *s = numtostr(i);
        mas[i] = malloc((strlen(s) + 1) * sizeof(char));
        strcpy(mas[i], s);
        mas[i][strlen(s)] = '\0';
        free(s);
    }
    mas[n + 1] = NULL;
    if(!fork()) {
        execvp(mas[0], mas);
        exit(1);
    } else wait(0);
    //printf("%s\n", mas[1]);
    i = 0;
    while(mas[i]) {
        //printf("%d\n", i);
        free(mas[i]);
        mas[i] = NULL;
        i++;
    }
    free(mas);
    return 0;
}
