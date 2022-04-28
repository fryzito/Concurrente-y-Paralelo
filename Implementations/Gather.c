#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc,char **argv) {
  int i,j,k,l;
  char iarray1[16];
  char iarray2[4];
  int myid, numprocs, ierr;
  MPI_Status status;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  printf("soy el nodo %d de %d procesos\n",myid,numprocs);
  for(i=0;i<16;i++)
    iarray1[i]='a';
  for(j=0;j<4;j++)
    iarray2[j]='a'+(myid*4+j);
  MPI_Gather(iarray2,4,MPI_CHAR,iarray1,4,MPI_CHAR,0,MPI_COMM_WORLD);
  if(myid==0)
    for(i=0;i<16;i++)
      printf(" %c",iarray1[i]);
  putchar('\n');
  MPI_Finalize();
  return 0;
}
