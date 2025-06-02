
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "statistics.h"

void compute_stats(double *seq, int n, const char *label) {
    double sum = 0.0, mean, std_dev = 0.0;

    for (int i = 0; i < n; i++) {
        sum += seq[i];
    }

    mean = sum / n;

    for (int i = 0; i < n; i++) {
        std_dev += (seq[i] - mean) * (seq[i] - mean);
    }

    std_dev = sqrt(std_dev / (n - 1));

    printf("%s\tMean: %.5f\tStd Dev: %.5f\n", label, mean, std_dev);
}

int main() {
    // Example small test
    int n = 5;
    double u[] = {1, 2, 3, 4, 5};
    double v[] = {2, 4, 6, 8, 10};

    compute_stats(u, n, "u");
    compute_stats(v, n, "v");

    return 0;
}
