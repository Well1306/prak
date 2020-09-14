#include <stdio.h>

void arol(int x[], int n, int k){      // сдвиг массива на 1 вправо
	int i;
	for (i=k; i > n; --i)
		 x[i] = x[i-1];
}

int main(void) {
	int x[1024]; int s=1; int j;       // x[s]-пустой 
	int p; int i;
	scanf("%d",&x[0]);
	while((s < 1024) && (scanf("%d", &p) == 1)){
		j = 0;
		while(p > x[j] && s != j) j++;
		if(j != s) arol(x,j,s);
		x[j] = p;
		s++;
	};
	for(i=0; i < s; ++i)
		printf("%d ", x[i]);
	return 0;
}
