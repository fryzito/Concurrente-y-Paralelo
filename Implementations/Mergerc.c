#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
void fusiona(int a[],int ini,int med,int fin){
  int size=fin-ini+1;
  int aux[size];
  int i = ini;
  int j = med+1;
  int k = 0;
  while(i<=med && j <= fin){
    if (a[i]<a[j]){
      aux[k]=a[i];
      i++;
    }
    else {
      aux[k]=a[j];
      j++;
    }
    k++;
  }
  while(i<=med){
    aux[k] = a[i];
    i++;
    k++;
  }
  while(j <= fin){
    aux[k] = a[j];
    j++;
    k++;
  }
  int n;
  for(n=0;n<size;++n)
    a[ini+n]=aux[n];
}
void mergersort(int a[],int ini,int fin) {
  if(ini < fin){
    int med = (ini+fin)/2;
    mergersort(a,ini,med);
    mergersort(a,med+1,fin);
    fusiona(a,ini,med,fin);
  }
}
int main(int argc,char **argv) {
  int i,j;
  int iarray1[16]={6,20,3,1,12,7,2,4,16,10,11,8,13,15,9,5};
  int iarray2[4];
  int myid, numprocs, ierr;
  MPI_Status status;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  printf("soy el nodo %d de %d procesos\n",myid,numprocs);
  mergersort(iarray1,myid*4,myid*4+3);
  for(j=0;j<4;j++)
    iarray2[j]=iarray1[myid*4+j];
  MPI_Gather(iarray2,4,MPI_INT,iarray1,4,MPI_INT,0,MPI_COMM_WORLD);
  int k;
  if(myid==0){
    mergersort(iarray1,0,15);
    for(k=0;k<16;k++)
      printf("%d ",iarray1[k]);
    putchar('\n');
  }
  MPI_Finalize();
  return 0;
}
