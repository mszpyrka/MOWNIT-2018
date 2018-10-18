main:	float_representation.c
	gcc float_representation.c -o main -lm -L/usr/local/include/gsl -lgsl -L/usr/local/include/gsl -lgslcblas

alg:	algorithm-1-4.c
	gcc algorithm-1-4.c -o zad4 -lm -L/usr/local/include/gsl -lgsl -L/usr/local/include/gsl -lgslcblas

kahan:	kahan.c
	gcc kahan.c -o kahan

clean:
	rm main zad4 kahan
