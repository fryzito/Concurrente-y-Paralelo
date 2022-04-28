#include <stdio.h>
#include <mpi.h>
#define MAXN 1000
int main(int argc,char **argv) {
  int rank,value,size;MPI_Status status;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  
  MPI_Finalize();
  return 0;
}
