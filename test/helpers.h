#include <stdbool.h>

#define EQUALBOUND 1e-15

bool equal(double a, double b) {
  double diff = a - b;
  if (diff < 0) diff = -diff;
  return diff < EQUALBOUND;
}
