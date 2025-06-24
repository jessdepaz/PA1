
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sequence_generator.h"
#include "statistics.h"

#define frand() (rand() / (double)RAND_MAX)
#define nrand() (sqrt(-2 * log(frand())) * cos(2 * M_PI * frand()))

void generate_scenario(double mu, double sigma, double m, double M, int n)
{
    double *u = malloc(n * sizeof(double));
    double *v = malloc(n * sizeof(double));
    double *t = malloc(n * sizeof(double));
    double *v_over_t = malloc(n * sizeof(double));
    double *expr = malloc(n * sizeof(double));

    // Sequence u
    for (int i = 0; i < n; i++)
    {
        u[i] = m + (M - m) * frand();
    }

    // Sequence v
    for (int i = 0; i < n; i++)
    {
        v[i] = mu + sigma * nrand();
    }

    // Sequence t (truncated normal)
    for (int i = 0; i < n;)
    {
        double temp = mu + sigma * nrand();
        if (temp >= m && temp < M)
        {
            t[i++] = temp;
        }
    }
    // v/t and expression
    for (int i = 0; i < n; i++)
    {
        v_over_t[i] = v[i] / t[i];
        expr[i] = 4 * u[i] - 5 * v[i] + 2 * t[i];
    }

    // Print results
    printf("Sequences for mu=%.2f, sigma=%.2f, m=%.2f, M=%.2f, n=%d\n", mu, sigma, m, M, n);
    printf("i\tu[i]\tv[i]\tt[i]\tv[i]/t[i]\t4u[i]-5v[i]+2t[i]\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%.5f\t%.5f\t%.5f\t%.5f\t%.5f\n", i, u[i], v[i], t[i], v_over_t[i], expr[i]);
    }

    // Compute and print statistics
    compute_stats(u, n, "u");
    compute_stats(v, n, "v");
    compute_stats(t, n, "t");
    compute_stats(v_over_t, n, "v/t");
    compute_stats(expr, n, "expr");

    free(u);
    free(v);
    free(t);
    free(v_over_t);
    free(expr);
}

int main()
{
    srand(time(NULL));

    // Scenario #1
    generate_scenario(5, 1, 1, 8, 20);

    // Scenario #2
    generate_scenario(pow(2, 10), pow(2, 8), 1, 2000, 200000);

    // Scenario #3
    generate_scenario(pow(2, 12), 1.3 * pow(2, 10), 1, 8100, 2000000);

    return 0;
}
