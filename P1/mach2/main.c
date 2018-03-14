#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "computation/omp.h"

int main(int argc, char ** argv) {
  unsigned int n;
  printf("Insert precisition n: ");
  scanf("%d", &n);
  time_t startTime = time(NULL);
  double res = piMachinOMP(n);
  printf("%.15f\n", res);
  printf("Wall time;%f\nError;%.15f\n", (double)(time(NULL) - startTime), M_PI - res);
  return 0;
}
