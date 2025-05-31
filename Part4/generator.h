// generator.h
#ifndef GENERATOR_H
#define GENERATOR_H

void generate_uniform(int *u, int n, int m, int M);
void generate_normal(double *v, int n, double mu, double sigma);
void generate_truncated_normal(int *t, int n, double mu, double sigma, int m, int M);

#endif
