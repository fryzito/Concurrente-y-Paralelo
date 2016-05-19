#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int conv2dSerial(const vector<vector<float>> &img,const vector<vector<float>> &kernel,const int &dim,const int &r){
  float sum;
  clock_t t;
  vector<vector<float>> result(dim);
  for (int i=0; i<dim; i++)
    result[i].resize(dim);
 
  t = clock();  
  for (int i=r; i<dim-r; i++){
    for (int j=r; j<dim-r; j++){
      sum = 0;
      for (int m=-r; m<=r; m++){
	for (int n=-r; n<=r; n++){
	  sum += img[i+m][j+n] * kernel[m+r][n+r];
	}
      }
      result[i][j] = sum;
    }
  }
  t = clock() - t;
  cout << "Tiempo convolucion 2D Secuencial: " << (float)t/CLOCKS_PER_SEC << endl;
  return 0;
}

int conv2dPar(const vector<vector<float>> &img, const vector<vector<float>> &kernel,const int &dim,const int &r){
  float sum;
  clock_t t;
  
  vector<vector<float>> result(dim);
  for (int i=0; i<dim; i++)
    result[i].resize(dim);
  t = clock();
  
  #pragma omp parallel for private(sum)
  for (int i=r; i<dim-r; i++){
    for (int j=r; j<dim-r; j++){
      sum = 0;
      for (int m=-r; m<=r; m++){
	for (int n=-r; n<=r; n++){
	  sum += img[i+m][j+n] * kernel[m+r][n+r];
	}
      }    
      result[i][j] = sum;
    }
  }
  t = clock() - t;
  printf("Tiempo convolucion 2D Paralelo: %f\n",(float)t/CLOCKS_PER_SEC );
  return 0;
}

void conv2d(){
  int dim = 15000;
  int r = 2;
  
  vector<vector<float>> img(dim);
  vector<vector<float>> kernel(2*r + 1);
  
  for (int i=0; i<dim; i++){
    img[i].resize(dim);
  }
  
  for (int i=0; i<2*r + 1; i++)
    kernel[i].resize(2*r + 1);
  
  conv2dSerial(img, kernel, dim, r);
  conv2dPar(img, kernel, dim, r);
}

int main(){
  conv2d();
  return 0;
}
