#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>
#include <limits.h>
#include <fcntl.h>
#define WR 1
#define APP 0

char** mas = NULL;
char* s = NULL;

void new_stdout(char* s, int mode) {
    int f;
    if(mode == WR) f = open(s, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(mode == APP)  f = open(s, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if(f == -1) perror("");
    close(1);
    dup2(f, 1);
    close(f);
}

void return_stdout(int old_out) {
    close(1);
    dup2(old_out, 1);
    close(old_out);
    old_out = dup(1);
}

void new_stdin(char *s) {
    int f = open(s, O_RDONLY);
    if(f == -1) perror("");
    close(0);
    dup2(f, 0);
    close(f);
}

void return_stdin(int old_in) {
    close(0);
    dup2(old_in, 0);
    close(old_in);
    old_in = dup(0);
}

void printdir(void) {
    char* st;
    char* s;
    st = malloc(sizeof(char) * PATH_MAX);
    s = getcwd(st, PATH_MAX);
    printf("%s$ ", s);
    free(st);
}

void delmas(int numb_m) {
    int j;
    for(j = 0; j < numb_m; j++) free(mas[j]);
    free(mas);
    mas = NULL;
}

int main() {
    char* home = getenv("HOME");                            //home dir
    char ch, flag;
    int numb_s = 0, size_s = 0;                             //word len
    int numb_m = 0, size_m = 0;                             //word cnt
    int i;
    char* redir_wa = NULL;
    char* redir_r = NULL;
    char* r_file = NULL;
    char* wa_file = NULL;
    int flag_r = -1; 
    int flag_w = -1;                                        //-1 -> no redir
    int flag_a = -1;                                        //-2 -> redir
    int old_stdout = dup(1);
    int old_stdin = dup(0);
    printdir();
    while((ch = getchar()) != EOF) {
       while(ch != '\n') {
           flag = (ch =='\"');
            while(flag || (ch != ' ')){
               if((ch != '\"') && (flag || ((ch != '>') && (ch != '<')))) {
                    if(numb_s == size_s) {
                            size_s = 2 * size_s + 1;
                            s = realloc(s, sizeof(char) *  size_s);
                            if(!s) {fprintf(stderr, "End of memory.\n"); return 1;}
                        }
                    s[numb_s] = ch;
                    numb_s++;
                }
                if(!flag) {
                    if(ch == '>') {
                        redir_wa = malloc(2); 
                        redir_wa[0] = ch; 
                        redir_wa[1] = '\0';
                        flag_w = numb_m + 1;
                        if(!s) flag_w--;
                        if((ch = getchar()) == '>') {
                            free(redir_wa); 
                            redir_wa = malloc(3);
                            redir_wa[0] = '>'; 
                            redir_wa[1] = '>'; 
                            redir_wa[2] = '\0'; 
                            ch = getchar();
                            flag_a = numb_m + 1;
                            if(!s) flag_a--;
                            flag_w = -1;
                            break;
                        }
                        else break;
                    }
                    if(ch == '<') {
                        flag_r = numb_m + 1;
                        if(!s) flag_r--;
                        redir_r = malloc(2); 
                        redir_r[0] = ch; 
                        redir_r[1] = '\0';
                        ch = getchar();
                        break;
                    }
                }
                if(((ch = getchar()) == EOF) || (ch == '\n')) break;
                if(ch == '\"') flag = 0;
            }
            if(s) {
                if(numb_s == size_s) {
                    s = realloc(s, sizeof(char) * (size_s + 1));
                    if(!s) {fprintf(stderr, "End of memory.\n"); return 1;}
                }
                s[numb_s] = '\0';
                if(numb_m == size_m) {
                    size_m++;
                    mas = realloc(mas, sizeof(*mas) * size_m);
                    if(!mas) {fprintf(stderr, "End of memory.\n"); return 2;}
                }
                if((numb_m != flag_r) && (numb_m != flag_w) && (numb_m != flag_a)) {
                    mas[numb_m] = malloc(sizeof(char) * (strlen(s) + 1));
                    strcpy(mas[numb_m], s);
                    numb_m++;
                } 
                else {
                    if(numb_m == flag_r) {
                        r_file = malloc(sizeof(char) * (strlen(s) + 1));
                        strcpy(r_file, s);
                        flag_r = -2;
                    }
                    else {
                        wa_file = malloc(sizeof(char) * (strlen(s) + 1));
                        strcpy(wa_file, s);
                        if(flag_w != -1) flag_w = -2;
                        if(flag_a != -1) flag_a = -2;
                    }
                }
            }
            free(s);
            s = NULL;
            numb_s = size_s = 0;
           while(ch == ' ') ch = getchar();
            if(ch == EOF) break;
        }
        mas = realloc(mas, sizeof(*mas) * (size_m + 1));
        mas[numb_m] = (char*) NULL;
        i = 0;
/*
        while(mas[i]) printf("%s!\n", mas[i++]);
        if(redir_r) {
            printf("%s ", redir_r);
            printf("%s?\n", r_file);
        }
        if(redir_wa) {
            printf("%s ", redir_wa); 
            printf("%s?\n", wa_file);
            printf("flag_w = %d\n", flag_w);
            printf("flag_a = %d\n", flag_a);
        } */
        /*cmd + args in mas*/
        if(flag_r == -2) new_stdin(r_file);
        if(flag_w == -2) new_stdout(wa_file, WR);
        if(flag_a == -2) new_stdout(wa_file, APP);
        if(mas[0]) {
            if(!strcmp(mas[0], "exit")) {
                delmas(numb_m);
                return_stdout(old_stdout);
                return_stdin(old_stdin);
                exit(0);
            } else {
                if(!strcmp(mas[0],"cd")) {
                    if(mas[1]) {
                        if(chdir(mas[1]) == -1) {
                            return_stdout(old_stdout);
                            return_stdin(old_stdin);
                            printf("%s: %s: No such file or directory\n", mas[0], mas[1]);
                        }
                    } else chdir(home);
                } else {
                    if(!fork()) {
                        //for(i = 0; i < numb_m; i++) fprintf(stderr, "%s ", mas[i]);
                       if(execvp(mas[0], mas) == -1) {
                            perror(mas[0]);
                            delmas(numb_m);
                            return_stdout(old_stdout);
                            return_stdin(old_stdin);
                            exit(0);
                        }
                    } else wait(0);
                }
            }
        }
        return_stdout(old_stdout);
        return_stdin(old_stdin); 
    printdir();
    delmas(numb_m);
    if(redir_r) {
        free(redir_r); 
        redir_r = NULL;
        free(r_file);
        r_file = NULL;
        flag_r = -1;
    }
    if(redir_wa) {
        free(redir_wa); 
        redir_wa = NULL;
        free(wa_file);
        wa_file = NULL;
        flag_w = flag_a = -1;
    }
    numb_m = size_m = 0;
    }
    return 0;
}
