#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <gsl/gsl_interp.h>
#include <gsl/gsl_ieee_utils.h>

#define FLOATING_PRECISION float
/*
FLOATING_PRECISION iterative_sum(FLOATING_PRECISION* array, int size) {

    FLOATING_PRECISION result = 0.;
    for(int i = 0; i < size; i++)
        result += array[i];

    return result;
}


FLOATING_PRECISION recursive_sum(FLOATING_PRECISION *array, int size) {

    if(size == 0)
        return 0.;

    if(size == 1)
        return array[0];

    int middle = size / 2;
    return recursive_sum(array, middle) + recursive_sum(array + middle, size-middle);
}

*/

FLOATING_PRECISION dumb_fraction_power(int numerator, int denominator, int power) {

    FLOATING_PRECISION fraction = ((FLOATING_PRECISION) numerator) / ((FLOATING_PRECISION) denominator);
    FLOATING_PRECISION result = 1.0;

    for(int i = 0; i < power; i++)
        result *= fraction;

    return result;
}


FLOATING_PRECISION precise_fraction_power(int numerator, int denominator, int power) {

    long long num = 1;
    long long den = 1;

    for(int i = 0; i < power; i++) {
        num *= numerator;
        den *= denominator;
    }

    FLOATING_PRECISION result = ((FLOATING_PRECISION) num) / ((FLOATING_PRECISION) den);

    return result;
}


FLOATING_PRECISION recursive_product(FLOATING_PRECISION *array, int size) {

    if(size == 0)
        return 1.;

    if(size == 1)
        return array[0];

    int middle = size / 2;
    return recursive_product(array, middle) * recursive_product(array + middle, size-middle);
}

int main() {

/*
    FLOATING_PRECISION a = 1.0/3.0;
    int size = 15;

    FLOATING_PRECISION *array = malloc(sizeof(FLOATING_PRECISION) * size);
    for(int i = 0; i < size; i++)
        array[i] = a;

    FLOATING_PRECISION iterative_sum_result = iterative_sum(array, size);
    FLOATING_PRECISION recursive_sum_result = recursive_sum(array, size);

    printf( "sum\n"
            "iterative:         %f\n"
            "recursive:         %f\n"
            "multiplication:    %f\n\n",
        iterative_sum_result, recursive_sum_result, size * a);
*/

    int numerator = 1;
    int denominator = 3;
    int power = 30;
    FLOATING_PRECISION dumb_result = dumb_fraction_power(numerator, denominator, power);
    FLOATING_PRECISION precise_result = precise_fraction_power(numerator, denominator, power);

    printf( "dumb:\t\t");
    gsl_ieee_fprintf_float(stdout, &dumb_result);
    printf( "\nrecursive:\t");
    gsl_ieee_fprintf_float(stdout, &precise_result);
    printf("\n");
}
