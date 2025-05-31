// stats.c
#include <stdio.h>
#include <math.h>
#include "stats.h"

double compute_mean_double(double *arr, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum / n;
}

double compute_stddev_double(double *arr, int n) {
    double mean = compute_mean_double(arr, n);
    double sum_sq_diff = 0.0;
    for (int i = 0; i < n; i++) {
        sum_sq_diff += (arr[i] - mean) * (arr[i] - mean);
    }
    return sqrt(sum_sq_diff / (n - 1));
}

double compute_mean_int(int *arr, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }
    return sum / n;
}

double compute_stddev_int(int *arr, int n) {
    double mean = compute_mean_int(arr, n);
    double sum_sq_diff = 0.0;
    for (int i = 0; i < n; i++) {
        sum_sq_diff += (arr[i] - mean) * (arr[i] - mean);
    }
    return sqrt(sum_sq_diff / (n - 1));
}
