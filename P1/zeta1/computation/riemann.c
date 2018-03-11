#include "riemann.h"
#include <math.h>

double piRiemannPart(unsigned long n){
  return 1.0/(n*n);
}

double piFromRiemann(double piApprox){
  return sqrt(piApprox*6);
}
