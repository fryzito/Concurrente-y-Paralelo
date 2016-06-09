#include <bits/stdc++.h>
#include <pthread.h>
#define MAXN 100
#define ROOT_PATH 0
#define thread_count 4
using namespace std;

pthread_t *thread_handles;
pthread_mutex_t lock;
pthread_mutex_t out;

vector<pair<int,int> > G[MAXN];
int n, num_cities;
struct tour_t {
  int A[MAXN];
  int city;
  int cost;
  tour_t(int pcity=0,int pcost=0){city=pcity;cost=pcost;}
};
vector<tour_t*> subtours;

queue<pair<tour_t*, int> > q;
tour_t *best_tour,*tour;

void Add_city(tour_t *tour,int city, int w) {
  tour->A[tour->city++] = city;
  tour->cost += w;
}

bool Best_tour(tour_t *tour) {
  bool res = best_tour->cost > tour->cost;
  return res;
}

bool Feasible(tour_t *tour, int city) {
  for(int i=0;i < tour->city;i++) {
    if(tour->A[i] == city) return false;
  }
  return true;
}

void Remove_last_city(tour_t *tour,int city, int w) {
  tour->A[tour->city - 1] = 0;
  tour->cost -= w;
  tour->city--;
}

void update_best_tour(tour_t *tour) {
  memcpy(best_tour, tour, sizeof(tour_t));
}
int city_count(tour_t *tour) {
  return tour->city;
}

queue<pair<tour_t*, int> > TSP_BFS(int nivel) {

  tour_t *tour = new tour_t;
  tour->cost = 0;
  tour->A[0] = ROOT_PATH;
  tour->city = 1;
  
  q.push(make_pair(tour, 0));
  int times = 0;
  while (!q.empty()) {
    int q_size = q.size();
    pair<tour_t*, int> ptour_t = q.front();
    tour_t *cur_tour = ptour_t.first;
    int depth = ptour_t.second;
    if (depth >= nivel) break;
    // NUM_THREADS
    q.pop();
    // seguimos expandiendo en BFS
    int city = cur_tour->A[cur_tour->city - 1];
    for (int nbr = 0; nbr < G[city].size(); nbr++) {
      int cur_city = G[city][nbr].first;
      int w = G[city][nbr].second;
      if(Feasible(cur_tour, cur_city)) {
        Add_city(cur_tour, cur_city, w);
        tour_t *new_tour = new tour_t;
        memcpy(new_tour, cur_tour, sizeof(tour_t));
        q.push(make_pair(new_tour, depth + 1));
        Remove_last_city(cur_tour, cur_city, w);
      }
    }
  }
  return q;
}
void *TSP_DFS(void* rank) {
  long my_rank = (long)rank;
  pthread_mutex_lock(&out);
  printf("Numero de Hilo thread = %ld total de hilos  %d Hilos estaticos\n", my_rank, thread_count);
  pthread_mutex_unlock(&out);
  int interval = ((int)subtours.size()) / thread_count;
  int start = my_rank * interval;
  int end = start + interval;

  for (int k = start; k < end; k++) {
    tour_t* tour = subtours[k];
    stack<tour_t*> l_stack;
    l_stack.push(tour);
    while (!l_stack.empty()) {
      tour_t *cur_tour = l_stack.top();
      l_stack.pop();
      if (city_count(cur_tour) == num_cities) {
        int w = 0, idx = -1, cur_city = cur_tour->A[cur_tour->city - 1];
        for (int i = 0; i < G[cur_city].size(); i++) {
          if (G[cur_city][i].first == ROOT_PATH) {
            idx = i; break;
          }
        }
        w = G[cur_city][idx].second;
        Add_city(cur_tour, ROOT_PATH, w);
        
        if(Best_tour(cur_tour)) {
          pthread_mutex_lock(&lock);
          if(Best_tour(cur_tour)) {
            // mostrar_path(cur_tour);
            update_best_tour(cur_tour);
          }
          pthread_mutex_unlock(&lock);
        
        }  
        Remove_last_city(cur_tour, ROOT_PATH, w);
      }
      else {
        int city = cur_tour->A[cur_tour->city - 1];
        
        for (int nbr = 0; nbr < G[city].size(); nbr++) {
          int cur_city = G[city][nbr].first;
          int w = G[city][nbr].second;
          if(Feasible(cur_tour, cur_city)) {
            Add_city(cur_tour, cur_city, w);
            tour_t *new_tour = new tour_t;
            memcpy(new_tour, cur_tour, sizeof(tour_t));
            l_stack.push(new_tour);
            Remove_last_city(cur_tour,cur_city, w);
          }
        }
      }
    }
  }
  
  // if this is the last thread
  if (my_rank == thread_count - 1) {
    pthread_mutex_lock(&out);
    pthread_mutex_unlock(&out);
    int start = thread_count * interval;
    int mod = ((int)subtours.size()) % thread_count;

    for (int k = start; k < start + mod; k++) {

      tour_t* tour = subtours[k];
      stack<tour_t*> l_stack;
      l_stack.push(tour);
      while (!l_stack.empty()) {
        tour_t *cur_tour = l_stack.top();
        l_stack.pop();
        if (city_count(cur_tour) == num_cities) {
          int w = 0, idx = -1, cur_city = cur_tour->A[cur_tour->city - 1];
          for (int i = 0; i < G[cur_city].size(); i++) {
            if (G[cur_city][i].first == ROOT_PATH) {
              idx = i; break;
            }
          }
          w = G[cur_city][idx].second;
          Add_city(cur_tour, ROOT_PATH, w);
          
          if(Best_tour(cur_tour)) {
            pthread_mutex_lock(&lock);    
            if(Best_tour(cur_tour)) {
              // mostrar_path(cur_tour);
              update_best_tour(cur_tour);
            }
            pthread_mutex_unlock(&lock);    
          
          }  
          Remove_last_city(cur_tour, ROOT_PATH, w);
        }
        else {
          int city = cur_tour->A[cur_tour->city - 1];
          
          for (int nbr = 0; nbr < G[city].size(); nbr++) {
            int cur_city = G[city][nbr].first;
            int w = G[city][nbr].second;
            if(Feasible(cur_tour, cur_city)) {
              Add_city(cur_tour, cur_city, w);
              tour_t *new_tour = new tour_t;
              memcpy(new_tour, cur_tour, sizeof(tour_t));
              l_stack.push(new_tour);
              Remove_last_city(cur_tour,cur_city, w);
            }
          }
        }
      }
    }
  }  
}

int main() {
  int times;
  long thread;
  scanf ("%d\n", &times);
  while (times--) {
    subtours.clear();
    scanf("%d %d\n",&n, &num_cities);

    // inicializar nodos
    fill(G, G + n, vector<pair<int,int> >());
    int a, b, w;
    // leyendo el grafo
    for(int i = 0; i < n; i++) {
      scanf("%d %d %d\n", &a, &b, &w);
      G[a].push_back(make_pair(b, w));
    }
    best_tour = new tour_t;
    best_tour->cost = 1 << 30;
    int nivel = num_cities / 2;
    queue<pair<tour_t*, int> > q = TSP_BFS(nivel);
    
    while (!q.empty()) {
      subtours.push_back(q.front().first);
      q.pop();
    }
    
    thread_handles = (pthread_t*)malloc(thread_count * sizeof(pthread_t));
    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&out, NULL);
    for (thread = 0; thread < thread_count; thread++) {
      pthread_create(&thread_handles[thread], NULL, TSP_DFS, (void*) thread);
    }
    for (thread = 0; thread < thread_count; thread++) {
      pthread_join(thread_handles[thread], NULL);
    }
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&out);
    
    printf("best_tour->cost %d\n", best_tour->cost);
  }
  return 0;
}
