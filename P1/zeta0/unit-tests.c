#include <stdio.h>
#include <stdbool.h>
#include "computation/serial.h"
#include "test/helpers.h"
#include "test/test-interface.h"

int main(int argc, char ** argv) {
  initTests();
  testEqual(piRiemannSerial(3), 2.857738033247041);
  return evaluateTests();
}
