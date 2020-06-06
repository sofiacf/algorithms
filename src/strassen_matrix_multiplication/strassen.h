#pragma once

#include <stdbool.h>
#include <stdlib.h>

bool ispowerof2(unsigned int x);

int add_matrices(const size_t n, const int x[n][n], const int y[n][n],
                 int output[n][n]);

int subtract_matrices(const size_t n, const int x[n][n], const int y[n][n],
                      int output[n][n]);

int print_matrix(const char* name, const int n, const int x[n][n]);

/*
 * Uses Strassen's to multiply matrices.
 *
 * constraints: n must be a power of 2
 */
int multiply_square_matrices(const size_t n, const int x[n][n],
                             const int y[n][n], int output[n][n]);

int brute_force(const size_t n, const int x[n][n], const int y[n][n],
                int output[n][n]);