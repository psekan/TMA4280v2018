#include "computation/zeta.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
  char *nProcs = getenv("OMP_NUM_THREADS");
  if(NULL == nProcs){
    printf("Enviroment variable OMP_NUM_THREADS is not set\n");
    return 1;
  }
  if((atoi(nProcs) & (atoi(nProcs)-1)) != 0){
    printf("OMP_NUM_THREADS needs to be set to power of 2\n");
    return 1;
  }
 
  int n; 
  printf("Insert precisition n: ");
  scanf("%d", &n);
  double result = zeta(n);
  printf("PI is: %.15f\n", result);
  return 0;
}
