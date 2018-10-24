#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>


gsl_matrix* get_random_matrix(int height, int width) {

    gsl_matrix *matrix = gsl_matrix_alloc(height, width);

    for(int i = 0; i < height * width; i++) {
        double value = ((double) (rand() % 50)) / ((double) (rand() % 50 + 1));
        gsl_matrix_set(matrix, i / width, i % width, value);
    }

    return matrix;
}


gsl_vector* get_random_vector(int size) {

    gsl_vector *vector = gsl_vector_alloc(size);

    for(int i = 0; i < size; i++) {
        double value = ((double) (rand() % 50)) / ((double) (rand() % 50 + 1));
        gsl_vector_set(vector, i, value);
    }

    return vector;
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

int main() {

    printf("\"operation\";\"size\";\"time\"\n");

    srand(time(NULL));

    int vector_sizes[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000};
    int vectors_number = 10;

    for(int i = 0; i < vectors_number; i++) {

        for(int j = 0; j < 10; j++) {

            double *result = malloc(vector_sizes[i] * sizeof(double));
            gsl_vector *vector_a = get_random_vector(vector_sizes[i]);
            gsl_vector *vector_b = get_random_vector(vector_sizes[i]);

            struct timeval tv_start, tv_end;
            gettimeofday(&tv_start, NULL);

            gsl_blas_ddot(vector_a, vector_b, result);

            gettimeofday(&tv_end, NULL);
            double time = subtract_time(tv_end, tv_start);

            printf("\"v o v\";%d;%f\n", vector_sizes[i], time);

            free(result);
            gsl_vector_free(vector_a);
            gsl_vector_free(vector_b);
        }
    }

    int matrix_sizes[] = {100, 500, 1000, 4000, 8000, 12000, 16000, 20000};
    int matrices_number = 8;

    for(int i = 0; i < matrices_number; i++) {

        for(int j = 0; j < 10; j++) {

            gsl_vector *result = gsl_vector_alloc(matrix_sizes[i]);
            gsl_vector *vector = get_random_vector(matrix_sizes[i]);
            gsl_matrix *matrix = get_random_matrix(matrix_sizes[i], matrix_sizes[i]);

            struct timeval tv_start, tv_end;
            gettimeofday(&tv_start, NULL);

            gsl_blas_dgemv(CblasNoTrans, 1.0, matrix, vector, 0.0, result);

            gettimeofday(&tv_end, NULL);
            double time = subtract_time(tv_end, tv_start);

            printf("\"m * v\";%d;%f\n", matrix_sizes[i], time);

            gsl_vector_free(result);
            gsl_vector_free(vector);
            gsl_matrix_free(matrix);
        }
    }
}
