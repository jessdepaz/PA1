// main.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generator.h"
#include "stats.h"

int main() {
    srand(time(NULL));  // seed RNG

    int n = 20, m = 1, M = 8;
    double mu = 5.0, sigma = 1.0;

    int u[n];
    double v[n];
    int t[n];

    generate_uniform(u, n, m, M);
    generate_normal(v, n, mu, sigma);
    generate_truncated_normal(t, n, mu, sigma, m, M);

    printf("u (Uniform):\n");
    for (int i = 0; i < n; i++) printf("%d ", u[i]);

    printf("\n\nv (Normal):\n");
    for (int i = 0; i < n; i++) printf("%.2f ", v[i]);

    printf("\n\nt (Truncated Normal Integers):\n");
    for (int i = 0; i < n; i++) printf("%d ", t[i]);

    printf("\n");

    return 0;
}
