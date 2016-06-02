#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#define G 6.673 * 1e-11
#define X 0
#define Y 1

void generarDatosDeEntrada(int num_cuerpos,int masses[],double vel_x[],double vel_y[],int pos_x[],int pos_y[]){
  int i;
  for(i=0;i<num_cuerpos;i++){
    masses[i] = rand()%10000000;
    pos_x[i] = rand()%100;
    pos_y[i] = rand()%100;
    vel_x[i] = (double)(rand()%10);
    vel_y[i] = (double)(rand()%10);
  }
  return;
}
int main(int argc,char **argv){
  int my_rank;

  int num_cuerpos=28;
  int n_steps = 10;
  int masses[num_cuerpos];
  int pos_x[num_cuerpos];
  int pos_y[num_cuerpos];
  double forces[num_cuerpos][2];
  double vel_x[num_cuerpos];
  double vel_y[num_cuerpos];

  
  MPI_Init(&argc,&argv);

  int size;
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&size);
  
  int loc_pos_x[num_cuerpos/size]; // arreglo auxiliar para pos pasar informacion x
  int loc_pos_y[num_cuerpos/size]; // arreglo auxiliar para pos pasar informacion y
  double loc_vel_x[num_cuerpos/size]; // arreglo auxiliar para vel pasar informacion x
  double loc_vel_y[num_cuerpos/size]; // arreglo auxiliar para vel pasar informacion y
  /*if (my rank == 0) */
  if(my_rank==0){
    /* for each particle */
    /*   Read masses[particle], pos[particle], vel[particle]; */
    generarDatosDeEntrada(num_cuerpos,masses,vel_x,vel_y,pos_x,pos_y);
    
    /* int i; */
    /* for(i=0;i<num_cuerpos;i++){ */
    /*   printf("i=%d vel(%lf,%lf)\n",i,vel_x[i],vel_y[i]); */
    /* } */

    /* putchar('\n'); */
  }
  /* MPI Bcast(masses, n, MPI DOUBLE, 0, comm); */
  /* MPI Bcast(pos, n, vect mpi t, 0, comm); */
  /* MPI Scatter(vel, loc n, vect mpi t, loc vel, loc n, vect mpi t, 0,comm); */

  MPI_Bcast(masses, num_cuerpos, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(pos_x, num_cuerpos, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(pos_y, num_cuerpos, MPI_INT, 0, MPI_COMM_WORLD);
  
  MPI_Scatter(vel_x, num_cuerpos/size , MPI_DOUBLE,loc_vel_x, num_cuerpos/size , MPI_DOUBLE, 0,MPI_COMM_WORLD);
  MPI_Scatter(vel_y, num_cuerpos/size , MPI_DOUBLE,loc_vel_y, num_cuerpos/size , MPI_DOUBLE, 0,MPI_COMM_WORLD);
  
  MPI_Scatter(pos_x, num_cuerpos/size , MPI_INT,loc_pos_x, num_cuerpos/size , MPI_INT, 0,MPI_COMM_WORLD);
  MPI_Scatter(pos_y, num_cuerpos/size , MPI_INT,loc_pos_y, num_cuerpos/size , MPI_INT, 0,MPI_COMM_WORLD);  

  /* for each timestep { */
  /*     for each local particle loc q */
  int q;

  for(q=0;q<num_cuerpos/size;q++){
    /*Compute total force on loc q; */
    int k;

    for(k=0;k<num_cuerpos;k++){
      if(pos_x[k]==loc_pos_x[q]) continue;
      int x_diff = loc_pos_x[q] - pos_x[k];
      int y_diff = loc_pos_y[q] - pos_y[k];
      double dist = sqrt(x_diff * x_diff + y_diff * y_diff);
      double dist_cubed = dist*dist*dist;
      if(dist_cubed==0) dist_cubed=1;
      forces[q][X] -= G*masses[q]*masses[k]/dist_cubed * x_diff;
      forces[q][Y] -= G*masses[q]*masses[k]/dist_cubed * y_diff;
    }
    
    /*     for each local particle loc q */
    /* 	   Compute position and velocity of loc q; */
    double delta_t = 10;

    //delta_t = step*delta_t;
    // for particle q
    // Compute position and velocity of q
    loc_pos_x[q] += delta_t * loc_vel_x[q];
    loc_pos_y[q] += delta_t * loc_vel_y[q];
    int sz = num_cuerpos/size;

    loc_vel_x[q] += delta_t/(double)masses[my_rank*sz+q]*forces[q][X];
    loc_vel_y[q] += delta_t/(double)masses[my_rank*sz+q]*forces[q][Y];
    //    printf("fuerza=%lf=%d\n",forces[q][Y],q);
  }
  /* Allgather local positions into global pos array; */
  
  /* Gather velocities onto process 0; */
  MPI_Gather(loc_vel_x,num_cuerpos/size,MPI_DOUBLE,vel_x,num_cuerpos/size,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Gather(loc_vel_y,num_cuerpos/size,MPI_DOUBLE,vel_y,num_cuerpos/size,MPI_DOUBLE,0,MPI_COMM_WORLD);
  
  MPI_Gather(loc_pos_x,num_cuerpos/size,MPI_DOUBLE,pos_x,num_cuerpos/size,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Gather(loc_pos_y,num_cuerpos/size,MPI_DOUBLE,pos_y,num_cuerpos/size,MPI_DOUBLE,0,MPI_COMM_WORLD);
  if (my_rank == 0) {
    /*   Print timestep; */
    /*   for each particle */
    /*      Print pos[particle] and vel[particle] */
    int i;
    for(i=0;i<num_cuerpos;i++){
	printf("posicion particula %d = (%d,%d) con velocidad (%f,%f)\n",i,pos_x[i],pos_y[i],vel_x[i],vel_y[i]);
    }
  }
  
  MPI_Finalize();
  return 0;
}
