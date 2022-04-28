#include <cstdio>
#include <iostream>
using namespace std;
int main(){
  int n,k,x=1,y=1;
  scanf("%d %d\n",&n,&k);
  for(int i=2;i<=k;i++)
    y*=i; 
  for(int i=n-k+1;i<=n;i++)
    x*=i;
  printf("%d/%d\n",x,y);
  return 0;
}
