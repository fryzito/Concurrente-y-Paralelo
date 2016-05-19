#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

void conv2dSerial(const vector<vector<float>> &img,const vector<vector<float>> &kernel,vector<vector<float>> &result,const int &dim,const int &r){
  float sum; 
  for (int i=r; i<dim-r; i++){
    for (int j=r; j<dim-r; j++){
      sum = 0;
      for (int m=-r; m<=r; m++){
        #pragma unroll
	for (int n=-r; n<=r; n++){
	  sum += img[i+m][j+n] * kernel[m+r][n+r];
	}
      }
      result[i][j] = sum;
    }
  }
}
void conv2dPar(const vector<vector<float>> &img,const vector<vector<float>> &kernel,vector<vector<float>> &result,const int &dim,const int &r){
  float sum;	
  #pragma omp parallel for private(sum)
  for (int i=r; i<dim-r; i++){
    for (int j=r; j<dim-r; j++){
      sum = 0;
      //#pragma loop(no_vector)
      #pragma unroll
      for (int m=-r; m<=r; m++){
	for (int n=-r; n<=r; n++){
	  sum += img[i+m][j+n] * kernel[m+r][n+r];
	}
      }
      result[i][j] = sum;
    }
  }
}
void conv2d(const int &N,const int &r){
  double t;
  vector<vector<float>> img, result, kernel;
  img.resize(N);
  result.resize(N);
  kernel.resize(2*r + 1);
  
  for (int i=0; i<N; i++)
    img[i].resize(N);
  
  for (int i=0; i<2*r + 1; i++)
    kernel[i].resize(2*r + 1);
  
  for (int i=0; i<N; i++)
    result[i].resize(N);
  
  printf("Algoritmo Secuencial\n");
  t = omp_get_wtime();
  conv2dSerial(img, kernel, result, N, r);
  t = omp_get_wtime() - t;
  
  printf("Tiempo de Ejecucion = %f\n\n",t);
  
  printf("Algoritmo Paralelo\n");
  t = omp_get_wtime();
  conv2dPar(img, kernel, result, N, r);
  t = omp_get_wtime() - t;
  printf("Tiempo de Ejecucion = %f\n",t);
  
  for (int i=0; i<img.size(); i++) img[i].clear();
  for (int i=0; i<result.size(); i++) result[i].clear();
  for (int i=0; i<kernel.size(); i++) kernel[i].clear();
  
  img.clear();
  result.clear();
  kernel.clear();
}

int main(){
  int N,r,threads;
  threads = 8;
  omp_set_num_threads(threads);
  
  // Instruccin para Hallar el numero de Hilos
  #pragma omp parallel
  {
    #pragma omp master
    {
      printf("Numero de Hilos = %d\n", omp_get_num_threads());
    }
  }
  
  //Convolution 2D
  N = 15000;
  r = 2;
  conv2d(N, r);
  return 0;
}
