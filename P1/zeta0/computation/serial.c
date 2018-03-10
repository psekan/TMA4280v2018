#include "base.h"
#include <math.h>

double riemannSerial(unsigned long n) {
    double s = 0;
    for (unsigned long i = 1; i <= n; i++) {
        s += riemannZetaV(i);
    }
    return s;
}

double piRiemannSerial(unsigned long n) {
    return sqrt(riemannSerial(n)*6);
}
