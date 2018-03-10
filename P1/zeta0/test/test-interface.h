#include <stdio.h>
#include <stdbool.h>

unsigned int tests, successTests;
void initTests() {
  tests = successTests = 0;
}

void testEqual(double computed, double expected) {
  tests++;
  if (equal(computed,expected)) {
    successTests++;
    return;
  }
  fprintf(stderr, "Error: expected value %.15f, computed value: %.15f.\n", expected, computed);
}

void testBetween(double value, double from, double to) {
  tests++;
  if ((to >= value && value >= from) || 
      (to <= value && value <= from)) {
    successTests++;
    return;
  }
  fprintf(stderr, "Error: value %.15f is not between %.15f and %.15f.\n", value, from, to);
}

int evaluateTests() {
  printf("%d/%d tests passed\n", successTests, tests);
  return (tests != successTests);
}
