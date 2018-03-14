#include "computation/zeta.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <math.h>
#include <math.h>

int main(int argc, char **argv){
  char *nThreads = getenv("OMP_NUM_THREADS");
  if(NULL == nThreads){
    printf("Enviroment variable OMP_NUM_THREADS is not set\n");
    return 1;
  }
  if((atoi(nThreads) & (atoi(nThreads)-1)) != 0){
    printf("OMP_NUM_THREADS needs to be set to power of 2\n");
    return 1;
  }
 
  int n; 
  printf("Insert precisition n: ");
  scanf("%d", &n);
  time_t startTime = time(NULL);
  double result = zeta(atoi(nThreads),n);
  printf("PI is: %.15f\n", result);
  printf("Wall time;%f\nError;%.15f\n", (double)(time(NULL) - startTime), M_PI - result);
  return 0;
}
