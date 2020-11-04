#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

char** mas;
char* s;

void delmas(int numb_m) {
    int j;
    for(j = 0; j < numb_m; j++) free(mas[j]);
    free(mas);
    mas = NULL;
}

int main() {
    char ch, flag;
    int numb_s = 0, size_s = 0;                             //длина слова 
    int numb_m = 0, size_m = 0;                             //количество слов
    int i;
    while((ch = getchar()) != EOF) {
        while(ch != '\n') {
            flag = (ch =='\"');
            while(flag || (ch != ' ')){
                    if(numb_s == size_s) {
                            size_s = 2 * size_s + 1;
                            s = realloc(s,sizeof(char) *  size_s);
                            if(!s) {fprintf(stderr, "End of memory.\n"); return 1;}
                    }
                    s[numb_s] = ch;
                    numb_s++;
                    if((ch = getchar()) == EOF) break;
                    if(ch == '\n') break;
                    if(ch == '\"') flag = 0;
            }
            if(numb_s == size_s) {
                s = realloc(s, sizeof(char) * (size_s + 1));
                if(!s) {fprintf(stderr, "End of memory.\n"); return 1;}
            }
            if(s) s[numb_s] = '\0';
            if(numb_m == size_m) {
                size_m++;
                mas = realloc(mas, sizeof(*mas) * size_m);
                if(!mas) {fprintf(stderr, "End of memory.\n"); return 2;}
            }
            mas[numb_m] = malloc(sizeof(char) * (strlen(s) + 1));
            strcpy(mas[numb_m], s);
            numb_m++;
            free(s);
            s = NULL;
            numb_s = size_s = 0;
            while(ch == ' ') ch = getchar();
            if(ch == EOF) break;
        }
        mas = realloc(mas, sizeof(*mas) * (size_m + 1));
        mas[numb_m] = (char*) NULL;
        /*команда с аргументами считана в массив mas*/
        if(!fork()) {
            if(execvp(mas[0], mas) == -1) fprintf(stderr, "Unknown command\n");
        }
        wait(0);
        printf("\n");
        delmas(numb_m);
        numb_m = size_m = 0;
    }
    return 0;
}
