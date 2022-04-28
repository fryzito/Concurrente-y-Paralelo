#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
int main(int argc,char **argv) {
  int p, np;
  int nAtoms;
  int *Distribucion=NULL;
  MPI_Status status;
  
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&p);
  MPI_Comm_size(MPI_COMM_WORLD,&np);
  if(p==0) {
    srand(32130); //cemilla para el rand
    Distribucion=(int *) malloc(np*sizeof(int));
    for(p=0;p<np;p++) {Distribucion[p]=rand()%10000;}
  } else {
    Distribucion = NULL;
  }
  
  MPI_Scatter(Distribucion,1,MPI_INT,&nAtoms,1,MPI_INT,0,MPI_COMM_WORLD);
  printf("En proceso %d hay %d  atomos\n",p,nAtoms);
  
  // if(p==0) 
  //delete(Distribucion);
  
  MPI_Finalize();
  return 0;
}
