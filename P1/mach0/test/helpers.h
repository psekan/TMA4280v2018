#include <stdbool.h>

#define EQUALBOUND 1e-15
#define PI 3.141592653589793 

bool equal(double a, double b) {
  double diff = a - b;
  if (diff < 0) diff = -diff;
  return diff < EQUALBOUND;
}

double differencePI(double a){
  double diff = PI - a;
  if (diff < 0) diff = -diff;
  return diff;
}
