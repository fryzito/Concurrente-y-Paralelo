#include <omp.h>
#include <stdio.h>
int main(){
  printf("Fuera de la region paraele\n");
  omp_set_num_threads(16);
  #pragma omp parallel
  {
    int id = omp_get_thread_num();
    int nt = omp_get_num_threads();
    printf("Sou a thread %d de un total %d \n",id, nt);
  }

  printf("Fuera de lar region paralela.\n\n");
  
  omp_set_num_threads(4);
  #pragma omp parallel
  {
    int id = omp_get_thread_num();
    int nt = omp_get_num_threads();
    printf("Soy el thread %d de un total %d \n",id,nt);
  }
  
  return 0;
}
