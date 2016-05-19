#include <bits/stdc++.h>
#define CLOCK print("clock %lf\n",(double)clock()/CLOCKS_PER_SEC);
using namespace std;

int main(){
  int N;
  scanf("%d\n",&N);
  int A[N][N];int B[N][N];
  //leendo matrices
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++) {
      scanf("%d",&A[i][j]);
    }
    scanf("\n");
  }
  //Multiplicando matrices
  clock_t start1=clock();   // <-- Tomando Tiempo
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++) {
      B[i][j]=0;
      for(int k=0;k<N;k++) {
	B[i][j]+=A[i][k]*A[k][j];
      }
    }
  }
  
  printf("Tiempo trascurrido: %f\n",((double)clock()-start1)/CLOCKS_PER_SEC);
  //Escribiendo matrices
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++) {
      printf(" %d",B[i][j]);
    }
    printf("\n");
  }
  
  return 0;  
}
