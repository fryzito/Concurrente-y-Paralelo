#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc,char **argv) {
  int my_rank;
  int n;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  if(my_rank==0) {
    printf("Hola, ingresa un numero: \n");
    scanf("%d\n",&n);
  }
  //envia datos a todos los demas procesos
  MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
  printf("Hola, soy el proceso %d y el numero es el %d\n",my_rank,n);
  MPI_Finalize();
  return 0;
}
