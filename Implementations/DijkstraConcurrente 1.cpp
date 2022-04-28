#include <iostream>
#include <climits>
#include <ctime>
#include <cstdlib>
#include <mpi.h>
using namespace std;
void print_mat(int *mat, int size) {
  cout << "DRUKUJE MACIERZ\n\n";
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++){
      cout << mat[i * size + j] << "\t";
    }
    cout << endl;
  }
}
void min(int *local_min,int *mark,int *pathestimate,const int &bottom,const int &top) {
  int min = INT_MAX;
  local_min[0] = INT_MAX;
  for(int i = bottom; i <= top; i++){
    if(mark[i]==0){
      if(min >= pathestimate[i]){
	min = pathestimate[i];  
	local_min[0] = min;
	local_min[1] = i;
      }
    }
  }
}
void update_distances(int *pathestimate, int *global_min, int *dyn_graph, int *mark, int size_of_graph, int bottom){
  int min_distance = global_min[0];
  int min_vertex = global_min[1];
  int temp_distance = 0;
  for (int i = 0; i < size_of_graph; i++) {       
    temp_distance = dyn_graph[min_vertex * size_of_graph +i];
    if(mark[i + bottom] == 0 && temp_distance > 0){
      if(pathestimate[i + bottom] > min_distance + temp_distance){
	pathestimate[i + bottom] = min_distance + temp_distance;
      }
    }
  }       
}
void draw_mat(int *mat, int size, int rank, int density, int size_of_graph){
  time_t t; 
  srand((unsigned) time(&t) * rank + 1); 
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size_of_graph; j++){
      if (i == j){
	mat[i * size_of_graph + j] = 0;
      }
      else {
	int vertex = rand() % 100;
	if (vertex <= density)
	  mat[i * size_of_graph + j] = rand() % 100;
	else
	  mat[i * size_of_graph + j] = 0;
      }
    }
  }
}
int main(int argc, char **argv) {
  int current_vertex;
  int count = 0;
  double init_start, init_end;
  int rank; 
  int process_count; // contador de procesos
  int size_of_graph; // Tanio del grafo
  int density;
  
  int *pathestimate;
  int *mark;
  int *dyn_graph;
  
  int source;
  int matrix_size ;
  int* global_min;
  int* local_min;
  
  double start_time;
  double end_time;
  int bottom = 0;
  int top = 0;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &process_count);
  if ( argc > 2 ){
    size_of_graph    = atoi(argv[1]);
    density          = atoi(argv[2]);
  }       
  else {
    if (rank == 0){
      cout << "la falta de argumentos de entrada - dar nueva:"
	   << endl << "tamanio del grafo: ";       
      cin >> size_of_graph;
      cout << "densidad grafo";
      cin >> density;
    }       
  }
  matrix_size = size_of_graph * process_count;
  if (rank == 0) {
    cout << "Columnas para el proceso:" <<  size_of_graph << " Tamaño del grafo " << matrix_size << endl;
    cout << "Maximo Entero: " << INT_MAX << endl;
    cout << "Memoria de un solo proceso: " << size_of_graph * matrix_size << endl;
    cout << "Densidad del grafo: " << density << endl;
  }
  init_start      =       MPI_Wtime();
  bottom          =       rank * size_of_graph;
  top             =       bottom + size_of_graph - 1;
  
  dyn_graph = new int[size_of_graph * matrix_size];
  draw_mat(dyn_graph, matrix_size, rank, density, size_of_graph);
  
  pathestimate = new int[matrix_size];
  mark = new int[matrix_size];
  
  local_min = new int[2];
  global_min = new int[2];
  
  source = 0;

  for(int i = 0; i < matrix_size; i++) {
    mark[i] = 0;
    pathestimate[i] = INT_MAX;
  }
  pathestimate[source] = 0;
  init_end=MPI_Wtime();
  if(rank==0)
    cout << "Tiempo para la inicialización " << init_end-init_start << " segundos" << endl;
  
  start_time = MPI_Wtime();
  
  if(rank==0)
    cout << " trabajan : \n";
  for(int i=0;i<process_count;++i){
    if(rank==i)
      cout << i << " ";
  }
  if(rank==0)
    cout << "Tamanio de la matriz " << matrix_size << endl;
  for(int i=1; i < matrix_size; ++i){
    min(local_min, mark, pathestimate, bottom, top);
    
    current_vertex = global_min[1];
    update_distances(pathestimate, global_min, dyn_graph, mark, size_of_graph, bottom);
    mark[global_min[1]] = 1;
  }
        
  int *recv_tab = new int[matrix_size];
  try {
    MPI_Gather(pathestimate + bottom, size_of_graph, MPI_INT, pathestimate, size_of_graph, MPI_INT,0, MPI_COMM_WORLD);
  }
  catch(exception &e) {
    cout << "blad" ;
  }
  end_time = MPI_Wtime();
  MPI_Finalize();
  delete [] dyn_graph ;
  delete [] pathestimate ;
  delete [] mark;
  delete [] recv_tab;
  
  if (rank == 0){
    cout << "\n\nEl algoritmo se lleva a cabo en " << end_time - start_time << endl;
    cout << "Tiempo de ejecución de todo el programa" << end_time - init_start << endl;
  }
  return 0;
}
