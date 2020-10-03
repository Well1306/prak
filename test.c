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
				if (strcmp(s1,s2) != 0) {
					printf("Файлы различаются в строке %d\n", i);
					printf("Строка первого файла: %s", s1);
					printf("Строка второго файла: %s", s2);
					goto cle;
				}
			}
			else 	if ((s1 == NULL) && (s2 == NULL)) {printf("Файлы идентичны\n"); goto cle;}
				else {
					printf("Один из файлов короче, различаются в строке %d\n", i);
					if (s1 != NULL) printf("Первый файл длиннее, последняя строка: %s", s1);
					if (s2 != NULL) printf("Второй файл длиннее, последняя строка: %s", s2);
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
