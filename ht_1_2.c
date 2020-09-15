#include <stdio.h>
#include <stdlib.h>
char* lenmax = NULL;
char* maslen = NULL;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
        int size_s = 0;
        int numb_s = 0, numb_max = 0;
        char ch;
        while((ch = getchar()) != EOF){
                while(ch != '\n'){
                        if(numb_s == size_s){
                                size_s = 2*size_s+1;
                                maslen = realloc(maslen,size_s);
                                if(maslen == NULL) return 1;
                        }
                        maslen[numb_s] = ch;
                        numb_s++;
                        if((ch = getchar()) == EOF) break;
                }
                if(numb_s > numb_max){
            lenmax = malloc(0);
                        lenmax = maslen;
                        numb_max = numb_s;
                }
        maslen = malloc(0);
                numb_s = size_s = 0;
        }
    int i;
        for(i = 0; i < numb_max; i++)
                putchar(lenmax[i]);
           putchar('\n');
    lenmax = malloc(0);
        return 0;
}
