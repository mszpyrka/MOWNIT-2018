#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <gsl/gsl_interp.h>
#include <gsl/gsl_ieee_utils.h>

int main() {

    float a = 1.0;
    while(a != 0.0) {
        printf("C rep: %f\t\tgsl rep:", a);
        gsl_ieee_fprintf_float(stdout, &a);
        printf("\n");
        a = a/2.0;
    }

    printf("C rep: %f\tgsl rep:", a);
    gsl_ieee_fprintf_float(stdout, &a);
    printf("\n");
}
