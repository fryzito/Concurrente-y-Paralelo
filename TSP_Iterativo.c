#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const int INFINITY = (1<<30);
const int NO_CITY = -1;

typedef int city_t;
typedef int weight_t;

typedef struct {
  city_t* cities;
  int count;
  weight_t cost;
} tour_t;

typedef struct stack_struct {
  tour_t* tour_p;               /* Tour Parcial             */
  city_t  city;                 /* City en consideracion    */
  weight_t cost;                /* Costo en ir a City    */
  struct stack_struct* next_p;  /* Siguiente Registro en la pila*/
} stack_elt_t;

/* Variables Globales */
int n;
int mat[100][100];
weight_t* mat_;
tour_t best_tour;

void Read_mat() {
  int i, j;
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      scanf("%d", &mat[i][j]);
}

void Print_mat() {
  int i, j;
  printf("Matriz de Orden = %d\n", n);
  printf("Matriz = \n");
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++)
      printf("%2d ", mat[i][j]);
    printf("\n");
  }
  printf("\n");
} 

void Inicializar_tour(tour_t* tour_p) {
  int i;

  tour_p->cities = malloc((n+1)*sizeof(city_t));
  for (i = 0; i <= n; i++) {
    tour_p->cities[i] = NO_CITY;
  }
  tour_p->cost = 0;
  tour_p->count = 0;
}
void Check_best_tour(city_t city, tour_t* tour_p) {
  int i;

  if (tour_p->cost + mat[city][0] < best_tour.cost) {
    for (i = 0; i < tour_p->count; i++)
      best_tour.cities[i] = tour_p->cities[i];
    best_tour.cities[n] = 0;
    best_tour.count = n+1;
    best_tour.cost = tour_p->cost + mat[city][0];
  }
}

tour_t* Dup_tour(tour_t* tour_p) {
  int i;
  tour_t* temp_p = malloc(sizeof(tour_t));
  temp_p->cities = malloc(n*sizeof(city_t));
  for (i = 0; i < n; i++)
    temp_p->cities[i] = tour_p->cities[i];
  temp_p->cost = tour_p->cost;
  temp_p->count = tour_p->count;
  return temp_p;
}  /* Dup_tour */ 

void Push(tour_t* tour_p, city_t city, weight_t cost, stack_elt_t** stack_pp) {
  stack_elt_t* temp = malloc(sizeof(stack_elt_t));
  temp->tour_p = Dup_tour(tour_p);
  temp->city = city;
  temp->cost = cost;
  temp->next_p = *stack_pp;
  *stack_pp = temp;
}

void Pop(tour_t** tour_pp, city_t* city_p, weight_t* cost_p,stack_elt_t** stack_pp) {
  stack_elt_t* stack_p = *stack_pp;
  *tour_pp = stack_p->tour_p;
  *city_p = stack_p->city;
  *cost_p = stack_p->cost;
  *stack_pp = stack_p->next_p;
}

void Search(void) {
  city_t nbr;
  city_t city;
  weight_t cost;
  tour_t* tour_p;
  stack_elt_t* stack_p;

  tour_p = malloc(sizeof(tour_t));
  Inicializar_tour(tour_p);
  // no poner el primer nodo, desde push duplicados
  stack_p = malloc(sizeof(stack_elt_t));
  stack_p->tour_p = tour_p;
  stack_p->city = 0;
  stack_p->cost = 0;
  stack_p->next_p = NULL;

  while (!Empty(stack_p)) {
    Pop(&tour_p, &city, &cost, &stack_p);
    tour_p->cities[tour_p->count] = city;
    tour_p->cost += cost;
    tour_p->count++;
    if (tour_p->count == n) {
      Check_best_tour(city, tour_p);
    } else {
      for (nbr = n-1; nbr > 0; nbr--)
	if (Feasible(city, nbr, tour_p))
	  Push(tour_p, nbr, mat[city][nbr], &stack_p);
    }
  }
}

int Feasible(city_t city, city_t nbr, tour_t* tour_p) {
  if (!Visited(nbr, tour_p) && tour_p -> cost + mat[city][nbr] < best_tour.cost)
    return 1;
  else
    return 0;
}

int Visited(city_t nbr, tour_t* tour_p) {
  int i;
  for (i = 0; i < tour_p->count; i++)
    if ( tour_p->cities[i] == nbr ) return 1;
  return 0;
}

void Print_tour(tour_t* tour_p, char* title) {
  int i;
  printf("%s:\n", title);
  for (i = 0; i < tour_p->count; i++)
    printf("%d ", tour_p->cities[i]);
  printf("\n\n");
}

int  Empty(stack_elt_t* stack_p) {
  if (stack_p == NULL)
    return 1;
  else
    return 0;
}

/*--------------------------------------------*/
int main(int argc, char* argv[]) {
  
  scanf("%d\n",&n);
  memset(mat,0,sizeof mat);
  Read_mat();  //Leer Matriz
  Print_mat(); // Escrbir Matriz

  Inicializar_tour(&best_tour);
  best_tour.cost = INFINITY;

  Search();
  Print_tour(&best_tour, "El Mejor Tour");
  printf("Costo = %d\n", best_tour.cost);
  
  return 0;
}
