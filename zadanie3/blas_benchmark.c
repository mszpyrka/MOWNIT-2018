#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>


gsl_matrix* get_random_gsl_matrix(int height, int width) {

    gsl_matrix *matrix = gsl_matrix_alloc(height, width);

    for(int i = 0; i < height * width; i++) {
        double value = ((double) (rand() % 50)) / ((double) (rand() % 50 + 1));
        gsl_matrix_set(matrix, i / width, i % width, value);
    }

    return matrix;
}


double **get_random_c_matrix(int height, int width) {

    double** matrix = malloc(height * sizeof(double*));
    matrix[0] = malloc(height * width * sizeof(double));

    for(int i = 1; i < height; i++)
        matrix[i] = matrix[i-1] + width;

    //for(int i = 0; i < height; i++)
    //    matrix[i] = (*matrix + width * i);

    //double (*matrix) [height] = malloc( sizeof(double[height][width]) );

    for(int i = 0; i < height * width; i++)
        matrix[i / width][i % width] = ((double) (rand() % 50)) / ((double) (rand() % 50 + 1));

    return matrix;
}

double **get_empty_c_matrix(int height, int width) {

    double** matrix = malloc(height * sizeof(double*));
    matrix[0] = malloc(height * width * sizeof(double));

    for(int i = 1; i < height; i++)
        matrix[i] = matrix[i-1] + width;

    for(int i = 0; i < height * width; i++)
        matrix[i / width][i % width] = 0.0;

    return matrix;
}

void free_c_matrix(double **matrix) {
    free(matrix[0]);
    free(matrix);
}

// Extracts time interval from two timeval structures
double subtract_time(struct timeval a, struct timeval b)
{
    double tmp_a = ((double) a.tv_sec)  + (((double) a.tv_usec) / 1000000);
    double tmp_b = ((double) b.tv_sec)  + (((double) b.tv_usec) / 1000000);
    return tmp_a - tmp_b;
}

// Prints time value in proper format
void print_time(double t)
{
    int minutes = (int) (t / 60);
    double seconds = t - minutes * 60;
    printf("%dm%.4fs\n", minutes, seconds);
}

// Different multiplication methods
void naive_multiplication(double **matrixA, double **matrixB, double **result, int size) {

    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            for(int k = 0; k < size; k++)
                result[i][j] += matrixA[i][k] * matrixB[k][j];
}

void better_multiplication(double **matrixA, double **matrixB, double **result, int size) {

    for(int i = 0; i < size; i++)
        for(int k = 0; k < size; k++)
            for(int j = 0; j < size; j++)
                result[i][j] += matrixA[i][k] * matrixB[k][j];
}



int main() {

    printf("func,size,time\n");

    srand(time(NULL));

    int sizes_small[] = {500, 750, 1000};
    int sizes_big[] = {1500, 2000, 2500, 3000, 3500};

    for(int i = 0; i < 3; i++) {

        for(int j = 0; j < 10; j++) {

            double **matrixA = get_random_c_matrix(sizes_small[i], sizes_small[i]);
            double **matrixB = get_random_c_matrix(sizes_small[i], sizes_small[i]);
            double **result = get_empty_c_matrix(sizes_small[i], sizes_small[i]);

            struct timeval tv_start, tv_end;
            gettimeofday(&tv_start, NULL);

            naive_multiplication(matrixA, matrixB, result, sizes_small[i]);

            gettimeofday(&tv_end, NULL);
            double time = subtract_time(tv_end, tv_start);

            printf("C naive -O3,%d,%f\n", sizes_small[i], time);

            free_c_matrix(result);
            free_c_matrix(matrixA);
            free_c_matrix(matrixB);
        }
    }

    for(int i = 0; i < 3; i++) {

        for(int j = 0; j < 10; j++) {

            double **matrixA = get_random_c_matrix(sizes_small[i], sizes_small[i]);
            double **matrixB = get_random_c_matrix(sizes_small[i], sizes_small[i]);
            double **result = get_empty_c_matrix(sizes_small[i], sizes_small[i]);

            struct timeval tv_start, tv_end;
            gettimeofday(&tv_start, NULL);

            better_multiplication(matrixA, matrixB, result, sizes_small[i]);

            gettimeofday(&tv_end, NULL);
            double time = subtract_time(tv_end, tv_start);

            printf("C better -O3,%d,%f\n", sizes_small[i], time);

            free_c_matrix(result);
            free_c_matrix(matrixA);
            free_c_matrix(matrixB);
        }
    }

    for(int i = 0; i < 3; i++) {

        for(int j = 0; j < 10; j++) {

            gsl_matrix *matrixA = get_random_gsl_matrix(sizes_small[i], sizes_small[i]);
            gsl_matrix *matrixB = get_random_gsl_matrix(sizes_small[i], sizes_small[i]);
            gsl_matrix *result = gsl_matrix_alloc(sizes_small[i], sizes_small[i]);

            struct timeval tv_start, tv_end;
            gettimeofday(&tv_start, NULL);

            gsl_blas_dgemm(
                CblasNoTrans,
                CblasNoTrans,
                1.0,
                matrixA,
                matrixB,
                0.0,
                result);

            gettimeofday(&tv_end, NULL);
            double time = subtract_time(tv_end, tv_start);

            printf("C blas -O3,%d,%f\n", sizes_small[i], time);

            gsl_matrix_free(result);
            gsl_matrix_free(matrixA);
            gsl_matrix_free(matrixB);
        }
    }

    // Big sizes:

    for(int i = 0; i < 5; i++) {

        for(int j = 0; j < 5; j++) {

            double **matrixA = get_random_c_matrix(sizes_big[i], sizes_big[i]);
            double **matrixB = get_random_c_matrix(sizes_big[i], sizes_big[i]);
            double **result = get_empty_c_matrix(sizes_big[i], sizes_big[i]);

            struct timeval tv_start, tv_end;
            gettimeofday(&tv_start, NULL);

            naive_multiplication(matrixA, matrixB, result, sizes_big[i]);

            gettimeofday(&tv_end, NULL);
            double time = subtract_time(tv_end, tv_start);

            printf("C naive -O3,%d,%f\n", sizes_big[i], time);

            free_c_matrix(result);
            free_c_matrix(matrixA);
            free_c_matrix(matrixB);
        }
    }

    for(int i = 0; i < 5; i++) {

        for(int j = 0; j < 5; j++) {

            double **matrixA = get_random_c_matrix(sizes_big[i], sizes_big[i]);
            double **matrixB = get_random_c_matrix(sizes_big[i], sizes_big[i]);
            double **result = get_empty_c_matrix(sizes_big[i], sizes_big[i]);

            struct timeval tv_start, tv_end;
            gettimeofday(&tv_start, NULL);

            better_multiplication(matrixA, matrixB, result, sizes_big[i]);

            gettimeofday(&tv_end, NULL);
            double time = subtract_time(tv_end, tv_start);

            printf("C better -O3,%d,%f\n", sizes_big[i], time);

            free_c_matrix(result);
            free_c_matrix(matrixA);
            free_c_matrix(matrixB);
        }
    }

    for(int i = 0; i < 5; i++) {

        for(int j = 0; j < 5; j++) {

            gsl_matrix *matrixA = get_random_gsl_matrix(sizes_big[i], sizes_big[i]);
            gsl_matrix *matrixB = get_random_gsl_matrix(sizes_big[i], sizes_big[i]);
            gsl_matrix *result = gsl_matrix_alloc(sizes_big[i], sizes_big[i]);

            struct timeval tv_start, tv_end;
            gettimeofday(&tv_start, NULL);

            gsl_blas_dgemm(
                CblasNoTrans,
                CblasNoTrans,
                1.0,
                matrixA,
                matrixB,
                0.0,
                result);

            gettimeofday(&tv_end, NULL);
            double time = subtract_time(tv_end, tv_start);

            printf("C blas -O3,%d,%f\n", sizes_big[i], time);

            gsl_matrix_free(result);
            gsl_matrix_free(matrixA);
            gsl_matrix_free(matrixB);
        }
    }
}
