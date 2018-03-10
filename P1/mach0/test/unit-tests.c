#include <stdio.h>
#include <stdbool.h>
#include "../machin/serial.h"
#include "../riemann/serial.h"
#include "helpers.h"
#include "test-interface.h"

bool testRiemannSerial() {
  double computed = piRiemannSerial(3);
  double expected = 2.857738033247041;
  if (equal(computed,expected)) {
    return true;
  }
  fprintf(stderr, "RiemannSerial test failed. Expected value: %.15f, Computed value: %.15f.\n", expected, computed);
  return false;
}

bool testMachinSerial() {
  double computed = piMachinSerial(3);
  double expected = 3.141621029325035;
  if (equal(computed,expected)) {
    return true;
  }
  fprintf(stderr, "MachinSerial test failed. Expected value: %.15f, Computed value: %.15f.\n", expected, computed);
  return false;
}

int main(int argc, char ** argv) {
  initTests();
  runTest(testRiemannSerial);
  runTest(testMachinSerial);
  return evaluateTests();
}
