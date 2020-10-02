#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define N 1024

int main(){
	FILE* f1 = fopen("aa1.txt", "r");
	FILE* f2 = fopen("aa2.txt", "r");
	char *s1 = malloc(N), *s2 = malloc(N);
	int i = 0;
	if ((f1 == NULL) || (f2 == NULL)) {printf("Один из файлов не удалось открыть"); goto clem;}
	else
	{
		while(1)
		{
			s1 = fgets(s1, N, f1);
			s2 = fgets(s2, N, f2);
			i++;
			if ((s1 != NULL) && (s2 != NULL)) 
			{
				printf("%d\n", strcmp(s1,s2));
				printf("%s", s1);
				printf("%s\n", s2);
				if (strcmp(s1,s2) != 0) {
					printf("файлы различаются в строке %d\n", i);
					printf("%s", s1);
					printf("%s", s2);
					goto cle;
				}
			}
			else 	if ((s1 == NULL) && (s2 == NULL)) {printf("файлы идентичны\n"); goto cle;}
				else {
					printf("один из файлов короче, различаются в строке %d\n", i);
					goto cle;
				}
		};
	}
cle:	fclose(f1);
	fclose(f2);
clem:	free(s1);
	free(s2);
	return 0;
}
