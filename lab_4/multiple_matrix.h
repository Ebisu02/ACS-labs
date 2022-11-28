//
// Created by hello on 28.11.2022.
//

#ifndef BENCHMARK_MULTIPLE_MATRIX_H
#define BENCHMARK_MULTIPLE_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <chrono>

using namespace std;

void multi_matrix(double *a, double *b, double *c, int n);
void init_matrix_mod(double *c, int n);
void matrix_multi_mod(double *a, double *b, double *c, int n);
void init_matrix(double *a, double *b, double *c, int n);
void do_lab_4(char** argv);

#endif //BENCHMARK_MULTIPLE_MATRIX_H
