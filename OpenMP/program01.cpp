#include <omp.h>
#include <stdio.h>
int main(){
  printf("Fuera de la region paraele\n");
  #pragma omp parallel
  {
    int id = omp_get_thread_num();
    int nt = omp_get_num_threads();
    printf("Sou a thread %d de un total %d \n",id, nt);
  }

  printf("Termino de la region paralela\n");
  return 0;
}
