#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "computation/serial.h"

int main(int argc, char ** argv) {
    if (argc == 1) {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }
    unsigned int n = atoi(argv[1]);

    double rzpi = piRiemannSerial(n);

    printf("RiemannZeta(%u) = %.15f\n", n, rzpi);
    return 0;
}
