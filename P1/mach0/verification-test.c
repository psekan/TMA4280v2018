#include <stdio.h>
#include <stdbool.h>
#include "computation/serial.h"
#include "test/helpers.h"
#include "test/test-interface.h"

int main(int argc, char ** argv) {
  initTests();
  unsigned int k = 1; 
  double diff = 0.0, lastDiff = PI;
  FILE *fp = fopen("../../Results/machinVerificationResult.txt", "w");
  if (fp == NULL) {
    fprintf(stderr,"Error opening file!\n");
    return 1;
  }
  fprintf(fp,"         n | error\n");
  fprintf(fp,"    ---------------\n");
  for (unsigned int i=1; i<=24; i++) {
    k *= 2;
    diff = differencePI(piMachinSerial(k));
    testBetween(diff, 0, lastDiff);
    lastDiff = diff;
    fprintf(fp, "%10u | %.15f\n",k,diff);
  }
  evaluateTests();
  return 0;
}
