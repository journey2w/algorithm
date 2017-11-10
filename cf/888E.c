#include <stdio.h>
#include <stdlib.h>
int a[35], n, m, arr1[1<<20], arr2[1<<20];
int idx1, idx2;
int mycmp(const void* a, const void* b) { return *(int *)a - *(int *)b;}
void dfs(int i, int sum, int type){
	if (type && i==n) {arr2[idx2++] = sum; return;}
	else if (!type && i==n/2) {arr1[idx1++]= sum; return ; }

	dfs(i+1, (sum+a[i])%m , type);
	dfs(i+1, sum, type);
}
int main(){
	scanf("%d %d", &n, &m);
	for (int i=0; i<n; i++){
		scanf("%d", &a[i]);
		a[i]%=m;
	}
	qsort (a, n, sizeof(int), mycmp);

	dfs(0, 0, 0);
	dfs(n/2, 0, 1);
	arr1[idx1++]=0;
	arr2[idx2++]=0;

	for (int i=0; i<idx1; i++) arr1[i]%=m;
	for (int i=0; i<idx2; i++) arr2[i]%=m;

	qsort(arr1, idx1, sizeof(int), mycmp);
	qsort(arr2, idx2, sizeof(int), mycmp);

	int ans=0;
	for (int i=0; i<idx1; i++){
		for (int j=0; j<idx2; j++){
			int tmp= (arr1[i]+arr2[j])%m;
			if (tmp > ans) ans=tmp;
			while(j<idx2-1 && arr2[j]==arr2[j+1]) j++;
		}
		while( i<idx1-1 && arr1[i]==arr1[i+1]) i++;
	}

	printf("%d\n", ans);
}
