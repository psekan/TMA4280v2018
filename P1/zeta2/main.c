#include "zeta.h"
//#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  if(NULL == getenv("OMP_NUM_THREADS")){
    printf("Enviroment variable OMP_NUM_THREADS is not set");
    return 1;
  }
  if(atoi(getenv("OMP_NUM_THREADS")) % 2 != 0){
    printf("OMP_NUM_THREADS needs to be set to power of 2");
    return 1;
  }
  
  int n = atoi(argv[1]);
  double result = zeta(n);
  printf("PI is: %.15f\n", result);
  return 0;
}
