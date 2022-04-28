#include <stdio.h>r
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#define CLOCK print("clock %lf\n",(double)clock()/CLOCKS_PER_SEC);
int main(int argc,char **argv) {
  int rank,value,size;MPI_Status status;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  int A[100][100],B[100][100],i,j,k;
  //leendo matrices
  int n;
  scanf("%d\n",&n);
  for(i=0;i<n;i++){
    for(j=0;j<n;j++) {
      scanf("%d",&A[i][j]);
    }
    scanf("\n");
  }
  //Multiplicando matrices
  clock_t start1=clock();   // <-- Tomando Tiempo
  for(i=0;i<n;i++){
    for(j=0;j<n;j++) {
      B[i][j]=0;
      for(k=0;k<n;k++) {
	B[i][j]+=A[i][k]*A[k][j];
      }
    }
  }
  printf("Tiempo trascurrido: %f\n",((double)clock()-start1)/CLOCKS_PER_SEC);
  //Escribiendo matrices
  for(i=0;i<n;i++){
    for(j=0;j<n;j++) {
      printf(" %d",B[i][j]);
    }
    printf("\n");
  }
  MPI_Finalize();
  return 0;
}
