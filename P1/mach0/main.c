#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "computation/serial.h"

int main(int argc, char ** argv) {
  unsigned int n;
  printf("Insert precisition n: ");
  scanf("%d", &n);
  printf("%.15f\n", piMachinSerial(n));
  return 0;
}
