#include <stdio.h>
#include <stdbool.h>
#include "computation/serial.h"
#include "test/helpers.h"
#include "test/test-interface.h"

int main(int argc, char ** argv) {
  initTests();
  unsigned int k=1; // this number needs to be 1
  double diff;
  double lastDiff = PI;
  FILE *fp = fopen("../../Results/riemannVerificationResult.txt", "w");
  if (fp == NULL)
  {
    printf("Error opening file!\n");
    return 1;
  }
  fprintf(fp,"         n | error\n");
  fprintf(fp,"    ---------------\n");
  for (unsigned int i=1; i<=24; i++){
     k *= 2;
     diff = differencePI(piRiemannSerial(k));
     testBetween(diff, 0, lastDiff);
     fprintf(fp, "%10u | %.15f\n",k,diff);
  }

  evaluateTests();
  return 0;
}
