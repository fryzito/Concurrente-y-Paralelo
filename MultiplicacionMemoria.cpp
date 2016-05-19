#include <bits/stdc++.h>
#define CLOCK print("clock %lf\n",(double)clock()/CLOCKS_PER_SEC);
using namespace std;

int main(){
  int N;
  scanf("%d\n",&N);
  int A[N][N];int B[N][N]; int res[N][N];
  //leendo matrices
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++) {
      scanf("%d",&A[i][j]);
    }
    scanf("\n");
  }
  //Multiplicando matrices
  clock_t start1=clock();   // <-- Tomando Tiempo
  
  int i,j,k;
  
  double tmp[N][N];
  for (i = 0; i < N; ++i)
    for (j = 0; j < N; ++j)
      tmp[i][j] = A[j][i];
  for (i = 0; i < N; ++i)
    for (j = 0; j < N; ++j)
      for (k = 0; k < N; ++k)
	res[i][j] += A[i][k] * tmp[j][k];
    
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


// hacer conclusiones
// hacer un informe
// medir las estadis accesos a cache
// y nro de operaciones del procesador 
// Utilizar Valgrind
