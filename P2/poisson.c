#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define PI 3.14159265358979323846

double functionValue(int x, int y, int h, int globlaPos);
double **allocateMatrix(int n);

int *rowsPerProcess(int rows, int numProc){
  int *sizes = calloc(numProc, sizeof(int));
  int leftProc = numProc;
  int leftRows = rows;
  int tmp;
  while(leftProc>0){
    tmp = leftRows/leftProc; 
    if (leftRows%leftProc > 0) tmp++;
    sizes[numProc-leftProc] = tmp;
    leftProc--;
    leftRows -= tmp;
  }
  return sizes;
}

int *calculateGlobal(int* sizes, int numProc){
  int *globalPos = calloc(numProc, sizeof(int));
  globalPos[0] = 0;
  for (int i=1; i<numProc; i++){
    globalPos[i] = globalPos[i-1] + sizes[i-1];
  }
  return globalPos;
}

int main(int argc, char **argv){

  // problem size
  int n = atoi(argv[1]);
  int m = n-1;
  double h = 1.0/(double)m; 

  int rank, size;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double **G = allocateMatrix(m);

  // Calculate number of rows for each process
  int *sizes = rowsPerProcess(n,4);

  int *globalPos = calculateGlobal(sizes, 4);

  // Initialize G
  #pragma omp parallel for schedule(static)
  for (int i=0; i < sizes[rank]; i++){
    for (int j=0; j < m; j++){
      G[i][j] = h*h*functionValue(i,j,h,globalPos[rank]); 
    }
  }
  
  //printf("%d\n", sizes[0]);
  //printf("%d\n", globalPos[2]);

  return 0;
}

double functionValue(int i, int j, int h, int globalPos){
  int x = (j+1)*h;
  int y = (globalPos+i+1)*h;
  //somefunction
  return x*y;
}

double **allocateMatrix(int m){
  double **matrix;
  matrix = (double **)malloc(m * sizeof(double*));
  for (int i=0;i<m;i++){
    matrix[i] = (double *)malloc(m * sizeof(double));
  }
  return matrix;
}
