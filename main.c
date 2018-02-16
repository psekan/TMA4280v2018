#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double riemannZetaV(unsigned long i) {
    return 1.0/(i*i);
}

double riemannZeta(unsigned long n) {
    double s = 0;
    for (unsigned long i = 1; i <= n; i++) {
        s += riemannZetaV(i);
    }
    return s;
}

double machinXPower(double x, unsigned long i) {
    double r = x;
    for (unsigned long n = 1; n < 2*i-1; n++) {
        r *= x;
    }
    return r;
}

double machinArctan(unsigned long n,double x) {
    double s = 0;
    for (unsigned long i = 1; i <= n; i++) {
        s += (i%2 == 0 ? -1 : 1) * (machinXPower(x, i) / (2*i -1));
    }
    return s;
}

double piFromRiemannZeta(unsigned long n) {
    return sqrt(riemannZeta(n)*6);
}

double piFromMachin(unsigned long n) {
    return (4*machinArctan(n,1./5.0)-machinArctan(n,1.0/239.0))*4;
}

int main(int argc, char ** argv) {
    if (argc == 1) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }
    unsigned int n = atoi(argv[1]);

    double rzpi = piFromRiemannZeta(n);
    double mlpi = piFromMachin(n);

    printf("RiemannZeta(%u) = %.15f\n", n, rzpi);
    printf("Machin(%u) = %.15f\n", n, mlpi);
    return 0;
}