#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "../mach-base/base.h"

#define ROOT 0
#define TAG 100

int main(int argc, char ** argv) {
  int rank, nproc, subproc;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  subproc = nproc - 1;

  if ((subproc & (subproc - 1)) != 0) {
    if (rank == ROOT) {
      fprintf(stderr, "Number of processes must be 2^x+1\n");
    }
    return 1;
  }

  double partialResult[2], startTime;
  partialResult[0] = partialResult[1] = 0;

  if (rank == ROOT) {
    unsigned int n;
    printf("Insert precisition n: ");
    scanf("%d", &n);

    startTime = MPI_Wtime();
    unsigned int partSize = n / subproc;
    unsigned int nBiggerParts = n % subproc;
    unsigned int *arr = malloc(sizeof(unsigned int) * (partSize + 1));
    unsigned int nextI = 1;
    for (unsigned int p = 0; p < subproc; p++) {
      unsigned int pPartSize = partSize + (p < nBiggerParts);
      for (unsigned int i = 0; i < pPartSize; i++) {
        arr[i] = nextI;
        nextI++;
      }
      MPI_Send(&pPartSize, 1, MPI_UNSIGNED, p+1, TAG, MPI_COMM_WORLD);
      MPI_Send(arr, pPartSize, MPI_UNSIGNED, p+1, TAG, MPI_COMM_WORLD);
    }
    free(arr);
  }
  else {
    unsigned int size;
    MPI_Recv(&size, 1, MPI_UNSIGNED, ROOT, TAG, MPI_COMM_WORLD, &status);
    unsigned int *arr = NULL;
    if (size > 0) {
      arr = malloc(sizeof(unsigned int)*size);
      MPI_Recv(arr, size, MPI_UNSIGNED, ROOT, TAG, MPI_COMM_WORLD, &status);
    }

    double A = 1.0/5.0, B = 1.0/239.0, tmp;
    for (unsigned int i = 0; i < size; i++) {
      tmp = machinPart(A,arr[i]);
      partialResult[0] += tmp;
      tmp = machinPart(B,arr[i]);
      partialResult[1] += tmp;
    }
    if (arr != NULL) free(arr);
  }
  
  double results[2];
  MPI_Allreduce(partialResult, results, 2, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  if (rank == ROOT) {
    double result = (4*results[0]-results[1])*4;
    printf("%.15f\n", result);
    printf("Procs;%d\nWall time;%f\nError;%.15f\n", subproc, (MPI_Wtime() - startTime), fabs(M_PI - result));
  }
  MPI_Finalize();
  return 0;
}
