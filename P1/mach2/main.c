#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "computation/omp.h"

int main(int argc, char ** argv) {
  unsigned int n;
  printf("Insert precisition n: ");
  scanf("%d", &n);
  printf("%.15f\n", piMachinOMP(n));
  return 0;
}
