#include <bits/stdc++.h>
#define MAXN 100
#define dbg(x) cout<<#x<<"="<<x<<endl
#define dbg2(x,y) cout<<#x<<"="<<x<<" "<<#y<<"="<<y<<endl
#define dbg3(x,y,z) cout<<#x<<"="<<x<<" "<<#y<<"="<<y<<" "<<#z<<"="<<z<<endl
#define INF (1<<30)
using namespace std;
bool visit[MAXN];
vector<pair<int,int> > G[MAXN];

int mincosto=INF;
int nodoinicio;
int n,m;
void TSP(int nodo,int *costo,int nrovisitados,vector<int> v){

  if(nrovisitados == n and nodo == 0){
    mincosto=min(mincosto,*costo);
  }
  else {
    for(int i=0;i<G[nodo].size();i++){
      if(!visit[G[nodo][i].first]){
	
	visit[G[nodo][i].first]=1;
	int val2 = *costo+G[nodo][i].second;
	TSP(G[nodo][i].first,&val2,nrovisitados+1,v);
	visit[G[nodo][i].first]=0;
	
      }
    }
  }
}
int main(){
  int b,e,w;
  scanf("%d %d\n",&n,&m);
  while(m--){
    scanf("%d %d %d\n",&b,&e,&w);
    G[b].push_back(make_pair(e,w));
  }
  
  int val=0;
  int startcost=0;
  vector<int> v;
  memset(visit,0,sizeof(visit));
  TSP(val,&startcost,0,v);
  printf("Costo Minimo: %d\n",mincosto);

  return 0;
}
