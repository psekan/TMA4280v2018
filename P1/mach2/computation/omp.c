#include "../../mach-base/base.h"
#include <omp.h>

double machinArctanOMP(unsigned long n,double x) {
  double s = 0;

  #pragma omp parallel for reduction (+:s)
  for (unsigned long i = 1; i <= n; i++) {
    double machinTmp = machinPart(x, i);
    if (0.0 == machinTmp) break;
    else s += machinTmp;
  }
  return s;
}

double piMachinOMP(unsigned long n) {
  double a,b;
  #pragma omp parallel sections
  {
    #pragma omp parallel section
    {
      a = machinArctanOMP(n,1./5.0);
    }
    #pragma omp parallel section
    {
      b = machinArctanOMP(n,1.0/239.0);
    }
  }
  return (4*a-b)*4;
}
