#include <mpi.h>
#include <stdio.h>
#define NRA 3000                    // number of rows in matrix A
#define NCA 2000                    // number of columns in matrix A
#define NCB 400                     // number of columns in matrix B
#define MASTER 0                    // taskid of first task
#define FROM_MASTER 1               // setting a message type
#define FROM_WORKER 2               // setting a message type
int main(int argc, char *argv[]) {
  int numtasks,                   // number of tasks
    taskid,                     // task identifier
    source,                     // task id of message source
    dest,                       // task id of message destination
    mtype,                      // message type
    rows,                       // rows of matrix A sent to each worker
    averow, extra, offset,      // used to determine rows sent to each
    worker,
    i, j, k;                    // misc
  double a[NRA][NCA],             // matrix A to be multiplied
    b[NCA][NCB],             // matrix B to be multiplied
    c[NRA][NCB];             // result matrix C
  MPI_Status status;
  printf("debug_1\n");           // the execution did not come to this  point
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  if(taskid == MASTER) {
    printf("Number of worker tasks = %d\n", numtasks-1);
    // initialize matrix a and matrix b
    for(i=0; i<NRA; i++) {
      for(j=0; j<NCA; j++) {
	a[i][j] = i+j;
      }
    }
    for(i=0; i<NCA; i++) {
      for(j=0; j<NCB; j++) {
	b[i][j] = i*j;
      }
    }
    // send matrix data to worker processes
    averow = NRA/(numtasks-1);
    extra = NRA%(numtasks-1);
    offset = 0;
    mtype = FROM_MASTER;
    for(dest=1; dest<numtasks; dest++) {
      rows = (dest <= extra) ? averow+1 : averow;
      printf("   sending %d rows to task %d\n", rows, dest);
      MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
      MPI_Send(&a[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype,
	       MPI_COMM_WORLD);
      MPI_Send(&b, NCA*NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
      offset = offset + rows;
    }
  }
  else {
    // receive matrix data from master process
    mtype = FROM_MASTER;
    MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD,
	     &status);
    MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
    MPI_Recv(&a, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD,
	     &status);
    MPI_Recv(&b, NCA*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD,
	     &status);
  }
  if(taskid == MASTER) {
    // receive results from worker processes
   mtype = FROM_WORKER;
   for(source=1; source<numtasks; source++) {
     MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD,
	      &status);
     MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD,
	      &status);
     MPI_Recv(&c[offset][0], rows*NCB, MPI_DOUBLE, source, mtype,
	      MPI_COMM_WORLD, &status);
   }
  }
  else {
    // matrix calculation
    for(k=0; k<NCB; k++) {
      for(i=0; i<rows; i++) {
	c[i][k] = 0.0;
	for(j=0; j<NCA; j++) {
	  c[i][k] = c[i][k] + a[i][j]*b[j][k];
	}
      }
    }
    // send results to master process
    mtype = FROM_WORKER;
    MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
    MPI_Send(&c, rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
 }
  if(taskid == MASTER) {
    printf("Here is the result matrix\n\n");
    /*
      for(i=0; i<NRA; i++) {
      for(j=0; j<NCB; j++) {
      printf("%6.2f   ", c[i][j]);
      }
      printf("\n");
      }
    */
  }
  MPI_Finalize();
  return 0;
}
