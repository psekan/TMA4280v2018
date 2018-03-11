#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "computation/riemann.h"

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

  unsigned int n = atoi(argv[1]);

  MPI_Status status;
  MPI_Init(&argc, &argv);

  double partialResult;
  int tag = 100;
  int rootProcess = 0;
  int rank;
  int numProcs;
  int dataArray[n];
  double result = 0;
  //int elementsToSend;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  int avgElements = n/(numProcs-1);
  int localArray[avgElements + 1];

  if ( rank == rootProcess) {
    double startTime, endTime;
    startTime = MPI_Wtime(); 
    if (numProcs % 2 != 0) {
      printf("Number of processes needs to be power of 2.");
      return 1;
    }
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
      MPI_Send( &dataArray[globalRank], elementsToSend, MPI_INT, i+1, tag, MPI_COMM_WORLD);
      i++;
    }

    // collect results from each child process
    for (int i=1; i < numProcs; i++) {
      MPI_Recv(&partialResult, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
      result += partialResult;  
    }
    
    //write results to file
    FILE *f = fopen("../../Results/riemannParallel.txt", "w");
    if (f == NULL)
    {
      printf("Error opening file!\n");
      exit(1);
    }
    endTime = MPI_Wtime();
    fprintf(f, "n = %d,  np = %d, wall time = %f, error = %.15f\n", n, numProcs, endTime - startTime, PI - piFromRiemann(result));
    printf("Results were written to file riemannParallel.txt in Results directory\n");
    fclose(f);
  }
  else {
    // child processes
    int size;
    MPI_Recv( &size, 1, MPI_INT, rootProcess, tag, MPI_COMM_WORLD, &status);
    MPI_Recv( &localArray, size, MPI_INT, rootProcess, tag, MPI_COMM_WORLD, &status);
    partialResult = 0.0;
    for (unsigned int i=0; i < size; i++){
      partialResult += piRiemannPart(localArray[i]);
    }
    MPI_Send( &partialResult, 1, MPI_DOUBLE, rootProcess, tag, MPI_COMM_WORLD);
  }
  
  MPI_Finalize();
  return 0;
}
