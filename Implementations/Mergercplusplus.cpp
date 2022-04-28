#include <vector>
#include <iostream>
#include <cstdio>
#define dbg2(x,y) cout << #x << " = " << x << " " << #y << " = " << y << endl
using namespace std;
void fusiona(vector<int> &v, int ini, int med, int fin) {
    vector<int> aux(fin - ini + 1);
    int i = ini; // Índice de la parte izquierda
    int j = med + 1; // Índice de la parte derecha
    int k = 0; // Índice del vector aux
 
    while (i <= med && j <= fin) {
        if (v[i] < v[j]) {
            aux[k] = v[i];
            i++;
        }
        else {
            aux[k] = v[j];
            j++;
        }
        k++;
    }
    while (i <= med) {
        aux[k] = v[i];
        i++;
        k++;
    }
 
    while (j <= fin) {
        aux[k] = v[j];
        j++;
        k++;
    }
    for (int n = 0; n < aux.size(); ++n) {
      v[ini + n] = aux[n];
    }
}
 
void merge_sort(vector<int> &v, int ini, int fin) {
    if (ini < fin) {

        int med = (ini + fin)/2;
        merge_sort(v, ini, med);
        merge_sort(v, med + 1, fin);
        fusiona(v, ini, med, fin);
    }
}

int main() {
  int a[] = {6,5,3,1,8,7,2,4};
  vector<int> v(a,a+8);
  merge_sort(v,0,7);
  for(int i=0; i<8; i++) {
    printf("%d ",v[i]);
  }
  printf("\n");
  return 0;
}
