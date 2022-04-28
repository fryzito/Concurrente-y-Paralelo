#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#define FROM_WORKER 1
#define FROM_MASTER 0
int main(int argc, char *argv[])
{
	int nsize, rank, rc, i, j, k,numworkers,source,dest,mtype,rows,averow, extra, offset;
	int NRA=10, NCA=10, NCB=10, MASTER=0;

	double a[NRA][NCA],b[NCA][NCB],c[NRA][NCB];
	int taskid,numtasks;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	if(numtasks< 2 ) {
		printf("Necesita al menos dos tareas MPI.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
		exit(1);
	}
	numworkers= numtasks-1;
	if (taskid== MASTER)
	 {
		 printf("mpi comenzo con %d nodos.\n",numtasks);
		 printf("Inicializando arreglos\n");
		 printf("Arreglo A\n");
		 for(i=0; i<NRA; i++)
		{
			 for(j=0; j<NCA; j++)
			 {
				a[i][j]= i+j;
				printf("%d  ",i+j);
			 }
			printf("\n");
		}
		printf("Arreglo B\n");
		 for(i=0; i<NCA; i++)
		{
			 for(j=0; j<NCB; j++)
			 {
				b[i][j]= i*j;
				printf("%d  ",i*j);
			 }
			printf("\n");
		}
		/* Enviar matrix de datos a los esclavos*/
		averow= NRA/numworkers;
		extra = NRA%numworkers;
		offset = 0;
		mtype= FROM_MASTER;
		clock_t tiempo = clock();     // <----Tomando tiempo
		for(dest=1; dest<=numworkers; dest++)
		{
			rows= (dest<= extra) ? averow+1 : averow;   
			printf("Enviando %d filas al nodo %d offset=%d\n",rows,dest,offset);
			MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
			MPI_Send(&a[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype,
			MPI_COMM_WORLD);
			MPI_Send(&b, NCA*NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
			offset = offset + rows;
		}
		/* Recivir Resultados desde los esclavos*/
		mtype= FROM_WORKER;
		for(i=1; i<=numworkers; i++)
		{
			source= i;
 			MPI_Recv(&offset, 1, MPI_INT, source, mtype, 
			MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, source, mtype, 
			MPI_COMM_WORLD, &status);
			MPI_Recv(&c[offset][0], rows*NCB, MPI_DOUBLE, source, 
			mtype, MPI_COMM_WORLD, &status);
			printf("Reciviendo resultados del nodo %d\n",source);
		}
		printf("Tiempo trascurrido: %f\n",((double)clock()-tiempo)/CLOCKS_PER_SEC);
		/* Printresults*/
		printf("******************************************************\n");
		printf("ResultMatrix:\n");
		for(i=0; i<NRA; i++)
		{
			printf("\n"); 
			for(j=0; j<NCB; j++) 
				printf("%6.2f   ", c[i][j]);
		}

		printf("\n******************************************************\n");
		printf("Done.\n");
	}
/**************************** workertask************************************/
	if(taskid> MASTER)
	{
		mtype= FROM_MASTER;
		MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&a, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		MPI_Recv(&b, NCA*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		for(k=0; k<NCB; k++)
		  for(i=0; i<rows; i++)
		    {
		      c[i][k] = 0.0;
		      for (j=0; j<NCA; j++)
			c[i][k] = c[i][k] + a[i][j] * b[j][k];
		    }
		mtype= FROM_WORKER;
		MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
		MPI_Send(&c, rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
