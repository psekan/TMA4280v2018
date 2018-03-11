#include "../../mach-base/base.h"

double machinArctanSerial(unsigned long n,double x) {
    double s = 0;
    double machinTmp;
    for (unsigned long i = 1; i <= n; i++) {
        machinTmp = machinPart(x, i);
        if (0.0 == machinTmp) break;
        else s += machinPart(x, i);
    }
    return s;
}

double piMachinSerial(unsigned long n) {
    return (4*machinArctanSerial(n,1./5.0)-machinArctanSerial(n,1.0/239.0))*4;
}
