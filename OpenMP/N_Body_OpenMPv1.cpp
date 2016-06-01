#include <bits/stdc++.h>
#define dbg(x) cout <<#x<<"="<<x<<endl
#define dbg3(x,y,z) cout <<#x<<"="<<x<<" "<<#y<<"="<<y<<" "<<#z<<"="<<z<<endl
#define G 6.673 * 1e-11
#define X 0
#define Y 1
using namespace std;
void generarDatosDeEntrada(int num_cuerpos,double masses[],double vel[][2],int pos[][2]){
  for(int i=0;i<num_cuerpos;i++){
    masses[i] = rand()%1000000;
    pos[i][X] = rand()%100;
    pos[i][Y] = rand()%100;
    vel[i][X] = rand()%10 * G;
    vel[i][Y] = rand()%10 * G;
  }
  return;
}
int main(){
  int num_cuerpos = 10;
  int n_steps = 10;
  double masses[num_cuerpos];
  int pos[num_cuerpos][2];
  double forces[num_cuerpos][2];
  double vel[num_cuerpos][2];
  // Get input data;
  generarDatosDeEntrada(num_cuerpos,masses,vel,pos);
  memset(forces,0,sizeof forces);
  // for each timestep
#pragma omp parallel for
  for(int step=1;step<=n_steps;step++){
    //     for each particle q
    #pragma omp parallel for
      for(int q=0;q<num_cuerpos;q++){
	//Compute total force on q;
	for(int k=0;k<num_cuerpos;k++){
	  //for each particle k != q
	  if(q==k) continue;
	  int x_diff = pos[q][X] - pos[k][X];
	  int y_diff = pos[q][Y] - pos[k][Y];
	  double dist = sqrt(x_diff * x_diff + y_diff * y_diff);
	  double dist_cubed = dist*dist*dist;
	  forces[q][X] -= G*masses[q]*masses[k]/dist_cubed * x_diff;
	  forces[q][Y] -= G*masses[q]*masses[k]/dist_cubed * y_diff;
	}
      }
      
      #pragma omp parallel for
      for(int q=0;q<num_cuerpos;q++){
	double delta_t = 0.5;
	delta_t = step*delta_t;
	// for particle q
	// Compute position and velocity of q
	pos[q][X] += delta_t * vel[q][X];
	pos[q][Y] += delta_t * vel[q][Y];
	vel[q][X] += delta_t/masses[q]*forces[q][X];
	vel[q][Y] += delta_t/masses[q]*forces[q][Y];	
      }
      
      // Print positions and velocities of particles;
      for(int q=0;q<num_cuerpos;q++){
	//if(q==4)
	printf("Particula %d Posición (%d,%d) con velocidad (%f,%f)\n",q,pos[q][X],pos[q][Y],vel[q][X],vel[q][Y]);
	//printf("step=%d particula %d = (%d,%d)\n",step,q,pos[q][X],pos[q][Y]);
      }
    }
  
  return 0;
}
