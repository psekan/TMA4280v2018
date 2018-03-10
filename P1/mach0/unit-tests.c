#include <stdio.h>
#include <stdbool.h>
#include "computation/serial.h"
#include "test/helpers.h"
#include "test/test-interface.h"

int main(int argc, char ** argv) {
  initTests();
  testEqual(piMachinSerial(1), 3.183263598326360);
  testEqual(piMachinSerial(2), 3.140597029326060);
  testEqual(piMachinSerial(3), 3.141621029325035);
  return evaluateTests();
}
