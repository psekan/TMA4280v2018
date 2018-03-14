#include "../../mach-base/base.h"
#include <omp.h>
#include <stdio.h>

double machinArctanOMP(unsigned long n,double x) {
  double s = 0;

  #pragma omp parallel for reduction (+:s)
  for (unsigned long i = 1; i <= n; i++) {
    double machinTmp = machinPart(x, i);
    s += machinTmp;
  }
  return s;
}

double piMachinOMP(unsigned long n) {
  double a,b;
  a = machinArctanOMP(n,1./5.0);
  b = machinArctanOMP(n,1.0/239.0);
  return (4*a-b)*4;
}
