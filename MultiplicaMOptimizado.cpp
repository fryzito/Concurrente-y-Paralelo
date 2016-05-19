#include <bits/stdc++.h>
#define CLOCK print("clock %lf\n",(double)clock()/CLOCKS_PER_SEC);
//#define SM (CLS / sizeof (double))
#define SM 5
using namespace std;

int main(){
  
  //printf("%d", SM);
  //return 0;
  putchar('1');  
  int N;
  printf("%d",1);
  scanf("%d\n",&N);
  printf("%d",1);
  int A[N][N];
  putchar('1');
  //leendo matrices
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++) {
      scanf("%d",&A[i][j]);
    }
    scanf("\n");
  }
  putchar('1');
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++) {
      //scanf("%d",&A[i][j]);
      A[i][j]= 0;
    }
    // scanf("\n");
  }
  
  printf("%d",1);
  int i,j,k,i2,k2,j2,res[N][N] ;
  int *rres, *mul1[N],*mul2[N],*rmul1, *rmul2;
  //Multiplicando matrices
  clock_t start1=clock();   // <-- Tomando Tiempo
  
  for (i = 0; i < N; i += SM)
    for (j = 0; j < N; j += SM)
      for (k = 0; k < N; k += SM)
	for (i2 = 0, rres = &res[i][j], rmul1 = &mul1[i][k]; i2 < SM;
	     ++i2, rres += N, rmul1 += N)
	  for (k2 = 0, rmul2 = &mul2[k][j];
	       k2 < SM; ++k2, rmul2 += N)
	    for (j2 = 0; j2 < SM; ++j2)
	      rres[j2] += rmul1[k2] * rmul2[j2];
  
  printf("Tiempo trascurrido: %f\n",((double)clock()-start1)/CLOCKS_PER_SEC);
  //Escribiendo matrices
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++) {
      printf(" %d",res[i][j]);
    }
    printf("\n");
  }
  
  return 0;  
}
