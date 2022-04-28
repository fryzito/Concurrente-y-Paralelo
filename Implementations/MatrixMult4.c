/*
 *  Parallel Matrix Multiplication Program
 *
 *  Doug Hoyte, 2003
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "mpi.h"


/* Timer for the *C*omputation */
double cstartwtime,cstopwtime;
/* Timer for how long the whole procedure *A*ctually took */
double astartwtime,astopwtime;


void printmatrix(FILE *outfp, int m, int n, float *mat) {

  int i,j;

  for(i=0; i<m; i++) {
    for(j=0; j<n; j++) fprintf(outfp, "%6g ", *(mat+(i*n)+j));
    fprintf(outfp, "\n");
  }

}



void doparamult(int Am, int An, int Bm, int Bn,
		float *matA, float *matB, float *matC) {
  float *currA, *currB, *tempmatA, *tempmatB, *summat;
  int i, j, trank, nprocs, num;
  int *distlist=NULL;
  MPI_Status stats;

  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &trank);

  // Allocate space on all processes
  tempmatA = (float *) malloc(Am * An * sizeof(float));
  tempmatB = (float *) malloc(Bm * Bn * sizeof(float));
  summat = (float *) malloc(An * Bn * sizeof(float));

  for(i=0; i < (An*Bn); i++) matC[i] = 0;
  for(i=0; i < (An*Bn); i++) summat[i] = 0;
  for(i=0; i < (Am*An); i++) tempmatA[i] = 0;
  for(i=0; i < (Bm*Bn); i++) tempmatB[i] = 0;

  if (trank == 0) {

    distlist = (int *) malloc((nprocs-1)*sizeof(int));
    for(i=0; i<(nprocs-1); i++) distlist[i] = 0;
    for(i=0; i<Am; i++) (*(distlist + (i%(nprocs-1))))++;

    currA=matA;
    currB=matB;

    for(i=1; i<nprocs; i++) {
      MPI_Send(distlist+(i-1), 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(currA, An*distlist[i-1], MPI_FLOAT, i, 0, MPI_COMM_WORLD);
      MPI_Send(currB, Bn*distlist[i-1], MPI_FLOAT, i, 0, MPI_COMM_WORLD);
      currA+=An*distlist[i-1];
      currB+=Bn*distlist[i-1];
    }

    free(distlist);
  } else {
    MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stats);
    MPI_Recv(tempmatA, Am*An, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &stats);
    MPI_Recv(tempmatB, Bm*Bn, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &stats);
  }

  if (trank != 0) {
    currA = tempmatA;
    currB = tempmatB;
    while(num) {

      for(i=0; i<An; i++)
        for(j=0; j<Bn; j++)
          *(summat+(i*Bn)+j) += currA[i] * currB[j];

      currA += An;
      currB += Bn;
      num--;
    }
  }

  MPI_Reduce(summat, matC, An*Bn, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  free(tempmatA);
  free(tempmatB);
  free(summat);

}



int multmatrices(char *fileA, char *fileB, char *fileC) {

  int Am=0, An=0, Bm=0, Bn=0;
  FILE *fpA, *fpB, *fpC;
  float *transmatA=NULL, *matB=NULL, *matC=NULL;
  int i, j, trank, nprocs;

  MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &trank);

  if (nprocs<2) {
    fprintf(stderr, "MUST BE RUN WITH AT LEAST 2 PROCESSES\n");
    return 0;
  }

  if (trank == 0) {
    fpA = fopen(fileA, "r");
    if (fpA == NULL) {
      fprintf(stderr, "UNABLE TO OPEN FILE '%s'\n", fileA);
      return 0;
    }
    fpB = fopen(fileB, "r");
    if (fpB == NULL) {
      fprintf(stderr, "UNABLE TO OPEN FILE '%s'\n", fileB);
      return 0;
    }

    fscanf(fpA, " %d ", &Am);
    fscanf(fpA, " %d ", &An);
    fscanf(fpB, " %d ", &Bm);
    fscanf(fpB, " %d ", &Bn);

    if (Am<=0 || An<=0 || Bm<=0 || Bn<=0) {
      fprintf(stderr, "UNABLE TO PARSE DIMENSIONS!\n");
      return 0;
    }

    if (An != Bm) {
      fprintf(stderr, "MATRICES ARE INCOMPATIBLE!\n");
      return 0;
    }

    transmatA = (float *) malloc(sizeof(float) * Am * An);
    matB = (float *) malloc(sizeof(float) * Bm * Bn);

    if(transmatA==NULL || matB==NULL) {
      fprintf(stderr, "INSUFFICIENT MEMORY!\n");
      return 0;
    }

    for (i=0; i<Am; i++)
      for (j=0; j<An; j++)
        fscanf(fpA, " %f ", (transmatA+(Am*j)+i) );

    for (i=0; i<(Bn * Bm); i++) fscanf(fpB, " %f ", matB+i);

    fclose(fpA);
    fclose(fpB);

  }

  MPI_Bcast(&An, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&Am, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&Bn, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&Bm, 1, MPI_INT, 0, MPI_COMM_WORLD);

  /* Allocate space on all processes for the matrix result.
     This makes it easy to code cause we can use Allreduce */
  matC = (float *) malloc(Am * Bn * sizeof(float));

  /* Start the timer */
  cstartwtime = MPI_Wtime();

  /* Notice the Am and An have been swapped because of the transposition */
  doparamult(An, Am, Bm, Bn, transmatA, matB, matC);

  /* Stop the timer */
  cstopwtime = MPI_Wtime();

  if (trank == 0) {
    if (*fileC != '-') {
      fpC = fopen(fileC, "w");
      if (fpC == NULL) {
        fprintf(stderr, "UNABLE TO OPEN '%s' FOR WRITING!\n", fileC);
        return 0;
      }
      fprintf(fpC, "%d %d\n", Am, Bn);
      printmatrix(fpC, Am, Bn, matC);
      fclose(fpC);
    } else {
      printf("A * B  =\n");
      printmatrix(stdout, Am, Bn, matC);
    }

  }

  return 1;

}


int main (int argc, char *argv[]) {

  int res,trank;
  char *f1=NULL, *f2=NULL, *f3=NULL;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &trank);

  if (trank == 0) {
    if (argc != 4) {
      fprintf(stderr, "Parallel Matrix Multiplication\n");
      fprintf(stderr, "   Usage: mpirun C %s <file A> <file B> <outfile>\n\n",argv[0]);
      fprintf(stderr, "   This program computes A * B matrix multiplication in parallel using MPI.\n");
      fprintf(stderr, "   If outfile is '-' then the answer will be printed on stdout.\n");
      MPI_Finalize();
      return 0;
    }
    f1 = argv[1];
    f2 = argv[2];
    f3 = argv[3];
  }

  astartwtime = MPI_Wtime();
  res = multmatrices(f1, f2, f3);
  astopwtime = MPI_Wtime();

  if (trank == 0) {
    if (res == 0) {
      fprintf(stderr, "\nMatrix Multiplication Failed...\n");
    } else {
      fprintf(stdout, "\nMatrix multiplication done.\n");
      fprintf(stdout, "%f seconds of computation, %f total seconds\n",
	      cstopwtime-cstartwtime, astopwtime-astartwtime);
    }
  }

  MPI_Finalize();

  return 0;

}
