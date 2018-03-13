#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../zeta1/computation/riemann.h"

#define PI 3.14159265358979323846

// count number of elements for each child process 
// distribute data uniformly
int dataDistribution(int numProcs, int n, int toAdd){ 
  float tmp;
  for (int i=0; i < n; i += toAdd){
    tmp = (n - i) % numProcs;
    numProcs--;
    if (tmp == 0) {
	return i;
    }
  }
  return 0;
}

int main(int argc, char **argv){

  MPI_Status status;
  MPI_Init(&argc, &argv);

  double partialResult;
  int tag = 100;
  int rootProcess = 0;
  int rank;
  int numProcs;
  double result=0;
  int n;
  double startTime, endTime;
  //int elementsToSend;i

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  if ( rank == rootProcess) {
    startTime = MPI_Wtime();
    if (((numProcs - 1) & (numProcs - 2)) != 0) {
      printf("Number of processes needs to be power of 2 + 1.");
      return 1;
    }
    //double result=0;
    printf("Insert precision n:\n");
    scanf("%d", &n);

    int avgElements = n/(numProcs-1);
    int *dataArray=malloc(sizeof(int)*n);

    int breakPoint = dataDistribution(numProcs-1, n, avgElements+1);

    //generating data vector
    for(unsigned i = 0; i < n; i++){
      dataArray[i] = i+1;
    }
   
    int i = 0;
    int elementsToSend;

    for(unsigned int globalRank=0; globalRank < n; globalRank+=elementsToSend){
      if (globalRank < breakPoint) elementsToSend = avgElements + 1;
      else elementsToSend = avgElements;

      // distribute data to each child process
      MPI_Send( &elementsToSend, 1, MPI_INT, i+1, tag, MPI_COMM_WORLD);
      MPI_Send( dataArray + globalRank, elementsToSend, MPI_INT, i+1, tag, MPI_COMM_WORLD);
      i++;
    }

    free(dataArray);
  }
  else {
    // child processes
    int size;
    MPI_Recv( &size, 1, MPI_INT, rootProcess, tag, MPI_COMM_WORLD, &status);

    int *localArray=malloc(sizeof(int)*size);

    MPI_Recv( localArray, size, MPI_INT,rootProcess, tag, MPI_COMM_WORLD, &status);

    partialResult = 0.0;
    for (unsigned int i=0; i < size; i++){
      partialResult += piRiemannPart(localArray[i]);
    }
    free(localArray);
  }
  
  MPI_Allreduce(&partialResult, &result, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  if ( rank == rootProcess) {
    endTime = MPI_Wtime();
    printf("%.15f\n", piFromRiemann(result));
    printf("Procs;%d\nWall time;%f\nError;%.15f\n", numProcs-1, endTime - startTime, PI - piFromRiemann(result));
  }

  MPI_Finalize();
  return 0;
}
