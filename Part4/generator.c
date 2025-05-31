// generator.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "generator.h"

// Uniform random integers between m and M
void generate_uniform(int *u, int n, int m, int M) {
    for (int i = 0; i < n; i++) {
        u[i] = rand() % (M - m + 1) + m;
    }
}

// Box-Muller transform for normal distribution
void generate_normal(double *v, int n, double mu, double sigma) {
    for (int i = 0; i < n; i += 2) {
        double u1 = (rand() + 1.0) / (RAND_MAX + 1.0);
        double u2 = (rand() + 1.0) / (RAND_MAX + 1.0);
        double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        double z1 = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);

        v[i] = mu + sigma * z0;
        if (i + 1 < n) v[i + 1] = mu + sigma * z1;
    }
}

// Truncated normal to integer range [m, M]
void generate_truncated_normal(int *t, int n, double mu, double sigma, int m, int M) {
    double temp;
    for (int i = 0; i < n; i++) {
        do {
            double u1 = (rand() + 1.0) / (RAND_MAX + 1.0);
            double u2 = (rand() + 1.0) / (RAND_MAX + 1.0);
            double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
            temp = mu + sigma * z;
        } while (temp < m || temp > M);
        t[i] = (int)round(temp);
    }
}
