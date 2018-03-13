#include "zeta.h"
#include <omp.h>
#include <math.h>
#include <stdio.h>

double zeta(int *array, int size)
{
  double sum = 0;
  
#pragma omp parallel for schedule(static) reduction(+:sum)
  for (int i = 0; i < size; i++)
  {
    // next lines were used for testing purposes
    //int thread = omp_get_thread_num();
    //printf("id: %d, i:%d\n", thread, array[i]);
    sum += 1.0/(array[i]*array[i]);
  }
  return sum;
}
