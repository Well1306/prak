#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int i = 0, maxfr = 0;
char *word = NULL, *prep = NULL;

struct tnode {
		char* wo;
		int count;
		struct tnode* left;
		struct tnode* right;
};

/////////////////////////////////////////////////////////////////////////

struct tnode* addnodew(struct tnode* Tree, char* w, int j) {
	if(Tree == NULL) {
		Tree = (struct tnode *)  malloc(sizeof(struct tnode));
		Tree->wo = (char *) malloc(sizeof(char) * (strlen(w)+1));
		strcpy(Tree->wo, w);
		Tree->count = 1;
		Tree->right = Tree->left = NULL;
	}
	else {	int b = strcmp(w, Tree->wo);
		if(b == 0) {
			Tree->count++;
			if(Tree->count > maxfr) maxfr = Tree->count;
		}
		else {	if(b < 0) Tree->left = addnodew(Tree->left, w, j);
			else Tree->right = addnodew(Tree->right, w, j);
		}
	}
	return Tree;
};

void deletetree(struct tnode *T) {
	if(T) {
		deletetree(T->left);
		deletetree(T->right);
		free(T->wo);
		free(T);
	}
};

void tprint(struct tnode* T, int s) {
	if (T != NULL) {
		tprint(T->left, s);
		printf("%s %d %f\n", T->wo, T->count, (double) T->count / s);
		tprint(T->right, s);
	}
};

void tprintfr(struct tnode* T, int s, int max) {
	if(T) { 
		if(T->count == max)
		if(T->count == max) printf("%s %d %f\n", T->wo, T->count, (double) T->count / s);
		tprintfr(T->left, s, max);
		tprintfr(T->right, s, max);
	}
}

int main(int argc, char* argv[])
{
	if(argc > 1) {
		if(strcmp(argv[1], "-i") == 0) freopen(argv[2], "r", stdin);
		else if(strcmp(argv[1], "-o") == 0) freopen(argv[2], "w", stdout);
		if(argc > 3) {if(strcmp(argv[3], "-o") == 0) freopen(argv[4], "w", stdout);}
	}
	int s = 0, flag = 0;
	char ch;
	struct tnode *T = NULL;
	while((ch = getchar()) != EOF) {
		while(isalnum(ch)) {
			if(!word) word = malloc(32);
			word[i] = ch;
			i++;
			ch = getchar();
		}
		if(word) word[i] = '\0';
		if(ispunct(ch)) {
			prep = malloc(sizeof(char) * 2);
			prep[0] = ch;
			prep[1] = '\0';
		}
		if(word) {
			T = addnodew(T, word, i);
		       	free(word);
			word = NULL; 
			s++;
		}
		if(prep) {T = addnodew(T, prep, 1); free(prep); prep = NULL; s++;}
		i = 0;
	}
	for(i = maxfr; i > 0; i--) tprintfr(T, s, i);
	deletetree(T);
	return 0;
}
