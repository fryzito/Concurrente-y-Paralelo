#include <cstdio>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <mpi.h>
#define MAXN 1000
using namespace std;
int INF = 0;
int n;
int dist[MAXN];
void Dijkstra(vector<pair<int,int> > lista[], int nodoInicial) {
  bool toc[n];
  memset(dist,120,sizeof(dist));
  memset(toc,0,sizeof(toc));
  dist[nodoInicial] = 0;
  int t = nodoInicial;
  for(int k = 0; k < n; k++) {
    toc[t] = true;
    for(int i = 0; i < lista[t].size(); i++)
      dist[lista[t][i].first] = min(dist[lista[t][i].first],dist[t]+lista[t][i].second);
    for(int i = 0; i < n;i++)
      if(toc[t] == true || (toc[i] == false && dist[i] < dist[t]))
        t = i;
  }
  return;
}
int main(int argc,char **argv) {
  int rank,value,size;MPI_Status status;
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  clock_t start = clock();  //<== Tomando Tiempo inicial
  int ini, to, w, nodoini,NroNodos;
  cin >> n;
  vector<pair <int,int> > lista[n];
  cin >> nodoini >> NroNodos;
  for(int i=0;i<NroNodos;i++) {
    cin >> ini >> to >> w;
    lista[ini].push_back(make_pair(to,w));
    lista[to].push_back(make_pair(ini,w));
    INF += w;
  }
  Dijkstra(lista,nodoini);
  for (int i = 0; i < n; i++) {
    printf("El nodo %d esta a una distance minima %d del nodo %d \n",i,dist[i],nodoini);
  }
  printf("El algorithmo Dijkstra se demoro %lf segundos\n",((double)clock()-start)/CLOCKS_PER_SEC);
  MPI_Finalize();
  return 0;
}
