#include "zeta.h"
#include <omp.h>
#include <math.h>
#include <stdio.h>

double zeta(int threads, int n)
{
  double sum = 0;
  
#pragma omp parallel for num_threads(4) schedule(static) reduction(+:sum)
  for (int i = 1; i <= n; i++)
  {
    // next lines were used for testing puroposes
    /* 
     int thread = omp_get_thread_num();
     int nthreads = omp_get_num_threads();
     printf("id: %d/%d, i:%d\n", thread, nthreads, i);
    */
    sum += 1.0/(i*i);
  }
  double result = sqrt(sum*6);
  return result;
}
