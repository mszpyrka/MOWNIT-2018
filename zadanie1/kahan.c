#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define FLOATING_PRECISION double

FLOATING_PRECISION iterative_sum(FLOATING_PRECISION* array, int size) {

    FLOATING_PRECISION result = 0.;
    for(int i = 0; i < size; i++)
        result += array[i];

    return result;
}


FLOATING_PRECISION kahan_sum(FLOATING_PRECISION *array, int size) {

    FLOATING_PRECISION error_compensator = 0.;
    FLOATING_PRECISION result = 0.;

    for(int i = 0; i < size; i++) {
        FLOATING_PRECISION component = array[i] - error_compensator;
        FLOATING_PRECISION tmp = result + component;
        error_compensator = tmp - result - component;
        result = tmp;
    }
    return result;
}



int main() {

    FLOATING_PRECISION a = 1.0/3.0;
    int size = 10000000;

    FLOATING_PRECISION *array = malloc(sizeof(FLOATING_PRECISION) * size);
    for(int i = 0; i < size; i++)
        array[i] = a;

    FLOATING_PRECISION iterative_result = iterative_sum(array, size);
    FLOATING_PRECISION kahan_result = kahan_sum(array, size);

    printf( "exact:\t\t%f\n", a * size);
    printf( "iterative:\t%f\n", iterative_result);
    printf( "kahan:\t\t%f\n", kahan_result);
}
