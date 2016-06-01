#include <cstdio>
#include <omp.h>
#include <iostream>
using namespace std;
int main(){
  printf("Empiezo de la region pararela\n");

  int id,nt;
  #pragma omp parallel private(id,nt)
  {
    id = omp_get_thread_num();
    nt = omp_get_num_threads();
    printf("Soy el thread %d de un total de %d hilos \n",id , nt);
   
  }
  printf("Estoy fuera de la region critica\n");
  return 0;
}
