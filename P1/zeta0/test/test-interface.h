#include <stdio.h>
#include <stdbool.h>

unsigned int tests, successTests;
void runTest(bool (*test)()) {
  tests++;
  if (test()) {
    successTests++;
  }
}

void initTests() {
  tests = successTests = 0;
}

int evaluateTests() {
  printf("%d/%d tests passed\n", successTests, tests);
  return (tests != successTests);
}
