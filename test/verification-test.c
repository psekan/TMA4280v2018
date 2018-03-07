#include <stdio.h>
#include <stdbool.h>
#include "../machin/serial.h"
#include "../riemann/serial.h"
#include "helpers.h"
#include "test-interface.h"

bool testRiemannSerialVerification(){
  unsigned int k=1; // this number needs to be 1
  double diff;
  FILE *fp = fopen("riemannVerificationResult.txt", "w");
  if (fp == NULL)
  {
    printf("Error opening file!\n");
    return false;
  }
  fprintf(fp,"         n | error\n");
  fprintf(fp,"    ---------------\n");
  for (unsigned int i=1; i<=24; i++){
     k *= 2;
     diff = differencePI(piRiemannSerial(k));
     fprintf(fp, "%10u | %.15f\n",k,diff);
  } 
  return true; 
}

bool testMachinSerialVerification(){
  unsigned int k=1; // this number needs to be 1
  double diff=0.0f;
  FILE *fp = fopen("machinVerificationResult.txt", "w");
  if (fp == NULL)
  {
    printf("Error opening file!\n");
    return false;
  }
  fprintf(fp,"         n | error\n");
  fprintf(fp,"    ---------------\n");
  for (unsigned int i=1; i<=24; i++){
     k *= 2;
     diff = differencePI(piMachinSerial(k));
     fprintf(fp, "%10u | %.15f\n",k,diff);
  }
  return true;
}


int main(int argc, char ** argv) {
  initTests();
  runTest(testRiemannSerialVerification);
  runTest(testMachinSerialVerification);
  return evaluateTests();
}
