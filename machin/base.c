double machinXPower(double x, unsigned long i) {
    double r = x;
    for (unsigned long n = 1; n < 2*i-1; n++) {
        r *= x;
    }
    return r;
}

double machinPart(double x, unsigned long i) {
    return (i%2 == 0 ? -1 : 1) * (machinXPower(x, i) / (2*i -1));
}
