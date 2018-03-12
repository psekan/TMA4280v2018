#include "zeta.h"
#include <omp.h>
#include <math.h>

double zeta(int n)
{
  double sum = 0;
  
#pragma omp parallel for schedule(static) reduction(+:sum)
  for (int i = 1; i <= n; i++)
    {
      sum += 1.0/(i*i);
    }
  double result = sqrt(sum*6);
  return result;
}
