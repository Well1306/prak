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
#define APP 0
#define WR 1

char*** mas = NULL;
int sig = 0;
int* proc = NULL;
int size_b = 0;

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

void printdir(void) {
    char* st;
    char* s;
    st = malloc(sizeof(char) * PATH_MAX);
    s = getcwd(st, PATH_MAX);
    printf("%s$ ", s);
    free(st);
}

void delmas(int i) {
    int j = 0;
    while(mas[i][j]) free(mas[i][j++]);
    free(mas[i]);
    mas[i] = NULL;
}

int separators(char ch) {
    return (ch == ';') + (ch == '|') + (ch == '&');
}

void wait1(int a) {
    signal(SIGUSR2, wait1);
    sig = 1;
}

void ctrl_c(int a) {
    signal(SIGINT, ctrl_c);
    printf("\n");
}

int arrisnull(int* x, int len) {
    int i = 0;
    int b = 0;
    for(i = 0; i < len; i++) {
        b = b + x[i];
    }
    return !b;
}

char** cmdcpy(char** src) {
    char** dest = malloc(sizeof(char*));
    int i = 0;
    dest[i] = NULL;
    while(src[i]) {
        dest[i] = malloc(strlen(src[i]) + 1);
        strcpy(dest[i], src[i]);
        i++;
        dest = realloc(dest, sizeof(char*) * (i + 1));
        dest[i] = NULL;
    }
    return dest;
}

void printc(char** cmd) {
    int i = 0;
    while(cmd[i]) printf("%s ", cmd[i++]);
    printf("\n");
}

void handler(int a) {
    signal(SIGCHLD, handler);
    pid_t pid;
    int status = 0;
    int i = 0, fnd = 0;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for(i = 0; i < size_b; i++) {
            if(pid == proc[i]) {
                fnd = 1;
                break;
            }
        }
        if(fnd) {
            fnd = 0;
            if(WIFEXITED(status))
                printf("\n[%d]+    Done    pid = %d    exitcode = %d\n", i + 1, pid, WEXITSTATUS(status));
            else if(WIFSIGNALED(status))
                printf("\n[%d]+    Killed    pid = %d    exitcode = %d\n", i + 1, pid, WEXITSTATUS(status));
            else if(WIFSTOPPED(status))
                printf("\n[%d]+    Stopped    pid = %d    exitcode = %d\n", i + 1, pid, WEXITSTATUS(status));
            else if(WSTOPSIG(status) == SIGTERM)
                printf("\n[%d]+    Terminated    pid = %d    exitcode = %d\n", i + 1, pid, WEXITSTATUS(status));
            proc[i] = 0;
            if(arrisnull(proc, size_b)) {
                size_b = 0;
                free(proc);
                proc = NULL;
            }
        }
    }
}


int main() {
    int error = 0;
    char* s = NULL;
    char* home = getenv("HOME");                            //home dir
    char ch, flag;
    int numb_s = 0, size_s = 0;                             //word len
    int numb_m = 0, size_m = 0;                             //word cnt
    int numb_c = 0, size_c = 0;
    int i;
    int conv = 0;                                           //0 -> no conv; 1 -> conv
    int fd[2];
    int back_flag = 0;
    pid_t pid;
    char** r_file = NULL;
    char** wa_file = NULL;
    char** seps = NULL;                                     //cmd[i] seps[i] cmd[i+1]
    int* flag_r = NULL;
    int* flag_w = NULL;                                     //-1 -> no redir
    int* flag_a = NULL;                                     //-2 -> redir
    int old_stdout = dup(1);
    int old_stdin = dup(0);
    signal(SIGUSR2, wait1);
    signal(SIGINT, ctrl_c);
    printdir();
    while((ch = getchar()) != EOF) {
        mas = malloc(sizeof(*mas));
        mas[0] = NULL;
        seps = malloc(sizeof(seps));
        seps[0] = NULL;
        r_file = malloc(sizeof(r_file));
        wa_file = malloc(sizeof(wa_file));
        r_file[0] = wa_file[0] = NULL;
        flag_r = malloc(sizeof(int));
        flag_r[0] = -1;
        flag_a = malloc(sizeof(int));
        flag_a[0] = -1;
        flag_w = malloc(sizeof(int));
        flag_w[0] = -1;
        while(ch != '\n') {
            while(!separators(ch)) {
                flag = (ch =='\"');
                while(flag || (ch != ' ')) {
                   if((ch != '\"') && (flag || ((ch != '>') && (ch != '<') && !separators(ch)))) {
                        if(numb_s == size_s) {
                                size_s = 2 * size_s + 1;
                                s = realloc(s, sizeof(char) *  size_s);
                                if(!s) {perror(""); return 1;}
                            }
                        s[numb_s] = ch;
                        numb_s++;
                    }
                    if(!flag) {
                        if(ch == '>') {
                            flag_w[numb_c] = numb_m + 1;
                            if(!s) flag_w[numb_c]--;
                            if((ch = getchar()) == '>') {
                                ch = getchar();
                                flag_a[numb_c] = numb_m + 1;
                                if(!s) flag_a[numb_c]--;
                                flag_w[numb_c] = -1;
                                break;
                            }
                            else break;
                        }
                        if(ch == '<') {
                            flag_r[numb_c] = numb_m + 1;
                            if(!s) flag_r[numb_c]--;
                            ch = getchar();
                            break;
                        }
                    }
                    if(((ch = getchar()) == EOF) || (ch == '\n')) break;
                    if(ch == '\"') flag = 0;
                    if(!flag && separators(ch)) break;
                }
                if(s) {
                    if(numb_s == size_s) {
                        s = realloc(s, sizeof(char) * (size_s + 1));
                        if(!s) {perror(""); return 1;}
                    }
                    s[numb_s] = '\0';
                    //printf("ERR\n");
                    if(numb_m == size_m) {
                        size_m++;
                        mas[numb_c] = realloc(mas[numb_c], sizeof(**mas) * size_m);
                        if(!mas) {perror(""); return 2;}
                    }
                    if((numb_m != flag_r[numb_c]) && (numb_m != flag_w[numb_c]) && (numb_m != flag_a[numb_c])) {
                        mas[numb_c][numb_m] = malloc(sizeof(char) * (strlen(s) + 1));
                        strcpy(mas[numb_c][numb_m], s);
                        numb_m++;
                    } else {
                            if(numb_m == flag_r[numb_c]) {
                                r_file[numb_c] = malloc(sizeof(char) * (strlen(s) + 1));
                                strcpy(r_file[numb_c], s);
                                flag_r[numb_c] = -2;
                            }
                            else {
                                wa_file[numb_c] = malloc(sizeof(char) * (strlen(s) + 1));
                                strcpy(wa_file[numb_c], s);
                                if(flag_w[numb_c] != -1) flag_w[numb_c] = -2;
                                if(flag_a[numb_c] != -1) flag_a[numb_c] = -2;
                        }
                    }
                    free(s);
                    s = NULL;
                    numb_s = size_s = 0;
                }
                while(ch == ' ') ch = getchar();
                if(mas) {
                    mas[numb_c] = realloc(mas[numb_c], sizeof(**mas) * (size_m + 1));
                    mas[numb_c][numb_m] = (char*) NULL;
                }
                if((ch == EOF) || (ch == '\n')) break;
            }
            numb_c++;
            //updating the arrays
            mas = realloc(mas, sizeof(*mas) * (numb_c + 1));
            seps = realloc(seps, sizeof(seps) * (numb_c + 1));
            seps[numb_c] = NULL;
            r_file = realloc(r_file, sizeof(r_file) * (numb_c + 1));
            wa_file = realloc(wa_file, sizeof(wa_file) * (numb_c + 1));
            r_file[numb_c] = wa_file[numb_c] = NULL;
            flag_r = realloc(flag_r, sizeof(int) * (numb_c + 1));
            flag_r[numb_c] = -1;
            flag_a = realloc(flag_a, sizeof(int) * (numb_c + 1));
            flag_a[numb_c] = -1;
            flag_w = realloc(flag_w, sizeof(int) * (numb_c + 1));
            flag_w[numb_c] = -1;
            mas[numb_c] = NULL;
            numb_m = size_m = 0;
            //printf("!%c!\n", ch);
            if((ch == EOF) || (ch == '\n')) break;
            if(separators(ch)) {
                seps[numb_c - 1] = malloc(sizeof(char) * 2);
                seps[numb_c - 1][0] = ch;
                ch = getchar();
                if(separators(ch)) {
                    if(seps[numb_c - 1][0] == ch) {
                        seps[numb_c - 1] = realloc(seps[numb_c - 1], sizeof(char) * 3);
                        seps[numb_c - 1][1] = ch;
                        seps[numb_c - 1][2] = '\0';
                        ch = getchar();
                    } else seps[numb_c - 1][1] = '\0';
                } else seps[numb_c -1][1] = '\0';
            }
        }
/*
        i = 0;
        while(seps[i]) {
            printf("%s! ", seps[i++]);
        }
        printf("\n");
        int j = 0;
        i = 0;
        printf("!!!numb_c = %d!!!\n", numb_c);
        while(mas[j]) {
            printf("---> j = %d:\n", j);
            while(mas[j][i]) printf("%s!\n", mas[j][i++]);
            j++;
        }
        if(redir_r) {
            printf("%s ", redir_r);
            printf("%s?\n",r_file);
        }
        if(redir_wa) {
            printf("%s ", redir_wa); 
            printf("%s?\n", wa_file);
            printf("flag_w = %d\n", flag_w);
            printf("flag_a = %d\n", flag_a);
        } */
        //for(i = 0; i < numb_c; i++) printf("r:%s %d ", r_file[i], *flag_r[i]);
        //printf("\n");
        //for(i = 0; i < numb_c; i++) printf("wa:%s w:%d a:%d ", wa_file[i], *flag_w[i], *flag_a[i]);

        /*cmd + args in mas*/
        for(i = 0; i < numb_c; i++) {
            if(flag_r[i] == -2) new_stdin(r_file[i]);
            if(flag_w[i] == -2) new_stdout(wa_file[i], WR);
            if(flag_a[i] == -2) new_stdout(wa_file[i], APP);
            if(conv) {
                conv = 0;
                conv_in(fd[0]);
            }
            if(seps[i]) {
                if(!strcmp(seps[i],"|")) {
                    conv = 1;
                    pipe(fd);
                    conv_out(fd[1]);
                }
                if(!strcmp(seps[i],"&")) {
                    back_flag = 1;
                    size_b++;
                    proc = realloc(proc, sizeof(int) * size_b);
                    proc[size_b - 1] = 0;
                    signal(SIGCHLD, handler);
                    if(mas[i + 1]) error = 1;
                }
            }
            if(error) {
                fprintf(stderr, "& not at the end.\n");
                for(i = i; i < numb_c; i++) {
                    delmas(i);
                    free(seps[i]);
                    seps[i] = NULL;
                    if(flag_r[i] == -2) {
                        free(r_file[i]);
                        r_file[i] = NULL;
                    }
                    if((flag_w[i] == -2) || (flag_a[i] == -2)) {
                        free(wa_file[i]);
                        wa_file[i] = NULL;
                    }
                }
                signal(SIGCHLD, SIG_DFL);
                break;
            }
            if(mas[i][0]) {
                if(!strcmp(mas[i][0], "exit")) {
                    for(i = i; i < numb_c; i++) {
                        delmas(i);
                        free(seps[i]);
                        free(r_file[i]);
                        free(wa_file[i]);
                    }
                    free(mas);
                    free(seps);
                    free(r_file);
                    free(wa_file);
                    free(flag_r);
                    free(flag_w);
                    free(flag_a);
                    return_stdout(old_stdout);
                    return_stdin(old_stdin);
                    free(proc);
                    printf("exit\n");
                    exit(0);
                } else {
                    if(!strcmp(mas[i][0],"cd")) {
                        if(mas[i][1]) {
                            if(chdir(mas[i][1]) == -1) {
                                return_stdout(old_stdout);
                                return_stdin(old_stdin);
                                printf("%s: %s: No such file or directory\n", mas[i][-1], mas[i][1]);
                            }
                        } else chdir(home);
                    } else {
                        if(!(pid = fork())) {
                            //for(i = 0; i < numb_m; i++) fprintf(stderr, "%s ", mas[i]);
                            if(!back_flag) signal(SIGINT, SIG_DFL);
                            else signal(SIGINT, SIG_IGN);
                            /*if(back_flag) {
                                //printf("[%d]  %d\n", size_b, proc[size_b - 1]);
                                while(!sig) printf("@");
                                sig = 0;
                            }*/
                            if(execvp(mas[i][0], mas[i]) == -1) {
                                perror(mas[i][0]);
                                for(i = i; i < numb_c; i++) {
                                    delmas(i);
                                    free(seps[i]);
                                    free(r_file[i]);
                                    free(wa_file[i]);
                                }
                                free(mas);
                                free(seps);
                                free(r_file);
                                free(wa_file);
                                free(flag_r);
                                free(flag_w);
                                free(flag_a);
                                return_stdout(old_stdout);
                                return_stdin(old_stdin);
                                free(proc);
                                exit(0);
                            }
                        } else {
                            if(back_flag) {
                                proc[size_b - 1] = pid;
                                printf("[%d] %d\n", size_b, proc[size_b - 1]);
                                //kill(pid, SIGUSR2);
                            }
                            else {
                                //signal(SIGCHLD, SIG_DFL);
                                waitpid(pid, NULL, 0);
                                //signal(SIGCHLD, handler);
                            }
                        }
                    }
                }
            }
            return_stdout(old_stdout);
            return_stdin(old_stdin);
            delmas(i);
            free(seps[i]);
            seps[i] = NULL;
            if(flag_r[i] == -2) {
                free(r_file[i]);
                r_file[i] = NULL;
            }
            if((flag_w[i] == -2) || (flag_a[i] == -2)) {
                free(wa_file[i]);
                wa_file[i] = NULL;
            }
            sig = 0;
        }
        error = 0;
        //printf("1");
        /*int k;
        for(k = 0; k < size_b; k++) {
            //printf("pid = %d, k = %d, kill = %d\n", proc[k], k + 1, kill(proc[k], SIGUSR1));
            if(proc[k]) {
               if(kill(proc[k], SIGUSR1) == -1) {
                   printf("[%d]+ Done, pid = %d     ", k + 1, proc[k]);
                   printc(cmd_b[k]);
                   int h = 0;
                   while(cmd_b[k][h]) free(cmd_b[k][h++]);
                   cmd_b[k] = NULL;
                   proc[k] = 0;
               }
            }
        }*/
        //printf("!%d\n", arrisnull(proc, size_b));
        if(arrisnull(proc, size_b)) {
            free(proc);
            proc = NULL;
            size_b = 0;
        }
        back_flag = 0;
        free(mas);
        mas = NULL;
        free(seps);
        seps = NULL;
        free(r_file);
        r_file = NULL;
        free(wa_file);
        wa_file = NULL;
        free(flag_r);
        flag_r = NULL;
        free(flag_w);
        flag_w = NULL;
        free(flag_a);
        flag_a= NULL;
        numb_c = size_c = 0;
        //signal(SIGCHLD, SIG_DFL);
        printdir();
    }
    if(proc) free(proc);
    return 0;
}
