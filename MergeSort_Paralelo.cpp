#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
#define SWAP(a,b) {int temp = a; a=b; b=temp;}
#define dbg(x) cout << #x <<"="<< x <<endl
#define SIZE 1048576
void setUp(int a[], int size);
void tearDown(double start,double end,int a[], int size,bool esparalelo);
void merge(int a[], int size, int temp[]);
void mergesort_serial(int a[], int size,int temp[]);
void mergesort_parallel_omp(int a[],int size, int temp[],int threads);

int main(){
  int a[SIZE];
  int temp[SIZE];
  double startTime, endTime;
  int num_threads;
  #pragma omp parallel
  {
  #pragma omp master
    {
      num_threads = omp_get_num_threads();
    }
  }

  setUp(a,SIZE);

  // Ejecucion algoritmo paralelo
  startTime = omp_get_wtime();
  mergesort_parallel_omp(a,SIZE, temp, num_threads);
  endTime = omp_get_wtime();
  
  tearDown(startTime, endTime, a, SIZE,1);
  
  //Ejecutando el Algoritmo Secuencial
  printf("\nAlgoritmo Secuencial\n");
  setUp(a,SIZE);

  startTime = omp_get_wtime();
  mergesort_serial(a,SIZE, temp);
  endTime = omp_get_wtime();
  tearDown(startTime, endTime, a, SIZE,0);


}

void setUp(int a[], int size){
  int i;
  
  srand(0);
  for (i=0; i<size; i++) {
    a[i] = rand() % size;
  }
  return;
}

void tearDown(double start, double end, int a[],int size,bool esparalelo){
  int sorted = 1;
  int i;
  
  printf("Tiempo de ejecucion: %f\n",end-start);
  
  for (i=0; i < size-1;i++){
    sorted &= (a[i] <= a[i+1]);
  }
  printf("Arreglo ordenado: %s\n",sorted?"SI":"NO");
  if(esparalelo){
    #pragma omp parallel
    {
      #pragma omp master
      {
	printf("Numero de Hilos: %d\n",omp_get_num_threads());
	printf("Numero de procesos: %d\n",omp_get_num_procs());
      }
    }
  } else {
    printf("Numero de Hilos: %d\n",1);
    printf("Numero de procesos: %d\n",1);    
  }
}

void merge(int a[],int size, int temp[]){
  int i1 = 0;
  int i2 = size / 2;
  int it = 0;

  while(i1 < size/2 && i2 < size) {
    if(a[i1]<=a[i2]){
      temp[it]=a[i1];
      i1 += 1;
    } else {
      temp[it]=a[i2];
      i2 += 1;
    }
    it += 1;
  }

  while(i1 < size/2){
    temp[it] = a[i1];
    i1++;
    it++;
  } 
  while(i2 < size){
    temp[it] = a[i2];
    i2++;
    it++;
  }

  memcpy(a, temp, size*sizeof(int));
}

void mergesort_serial(int a[],int size, int temp[]){
  int i;
  if(size == 2) {
    if(a[0] <= a[1])
      return;
    else {
      SWAP(a[0],a[1]);
      return;
    }
  }

  mergesort_serial(a,size/2,temp);
  mergesort_serial(a+size/2,size - size/2,temp);
  merge(a, size, temp);
}

void mergesort_parallel_omp(int a[],int size, int temp[],int threads){
  if(threads == 1) {
    mergesort_serial(a,size,temp);
  } else if (threads > 1){
    #pragma omp parallel sections
    {
      #pragma omp section
      mergesort_parallel_omp(a,size/2,temp,threads/2);
      #pragma omp section
      mergesort_parallel_omp(a+size/2,size-size/2,temp+size/2,threads-threads/2);
    }
    merge(a,size,temp);
  }
  
}

