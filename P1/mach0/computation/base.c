#define LOWERBOUND 1e-15

double machinXPower(double x, unsigned long i) {
    double r = x;
    for (unsigned long n = 1; n < 2*i-1; n++) {
        r *= x;
        if (r <= LOWERBOUND) return 0.0;
    }
    return r;
}

double machinPart(double x, unsigned long i) {
    return (i%2 == 0 ? -1 : 1) * (machinXPower(x, i) / (2*i -1));
}
