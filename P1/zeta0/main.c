#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "computation/serial.h"

int main(int argc, char ** argv) {
    unsigned int n;
    printf("Insert precisition n: ");
    scanf("%d", &n);
    double rzpi = piRiemannSerial(n);

    printf("RiemannZeta(%u) = %.15f\n", n, rzpi);
    return 0;
}
