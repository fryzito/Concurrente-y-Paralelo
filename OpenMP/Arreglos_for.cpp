#include <omp.h>
#include <stdio.h>
#define MAXN 16
int main(){
  
  float A[MAXN][MAXN];
  float B[MAXN][MAXN];
  float C[MAXN][MAXN];

  for(int i=0;i<MAXN;i++) {
    for(int j=0;j<MAXN;j++) {
      A[i][j] = 2;
      B[i][j] = 3;
    }
  }

  #pragma omp parallel
  {
    //int id = omp_get_thread_num();
    //int nt = omp_get_num_threads();
    //int size = (int) MAXN/nt;
    //int ini = id*size;
    //int fin = ini + size - 1;
    
    #pragma omp for
    for(int i=0;i<16;i++){
      for(int j=0;j<16;j++){  // Nao sera paralelizada
	C[i][j]=A[i][j]+B[i][j];
      }
    }
  }
  
  for (int i = 0; i < MAXN;i++) {
    for(int j=0;j<MAXN;j++){
      printf("%f%c",C[i][j] ,(char)j==MAXN-1?10:32);
    }
  }
  return 0;
}
