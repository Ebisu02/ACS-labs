//
// Created by hello on 27.09.2022.
//

#ifndef BENCHMARK_BENCHMARK_CPU_H
#define BENCHMARK_BENCHMARK_CPU_H

#include <iostream>
#include <array>
#include <string>
#include <intrin.h>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <valarray>

//TODO COUNTER FOR AMOUNT OF OPERATIONS
// NOW IT'S CONST EQUALS 30000

#define li long int
#define ld long double
#define ui unsigned int

using namespace std;

double meanSquareDeviation(double* array, int n, char combination);
string getCpuBrand();
void matrixVectorProduct(double *a, double *b, double *c, int m, int n);
void matrixVectorProduct(int *a, int *b, int *c, int m, int n);
void matrixVectorProduct(float *a, float *b, float *c, int m, int n);
void printFile(double AvTime, double AbsError, double RelError, double TaskPerf, double time_one);
void benchmark_cpu();

#endif //BENCHMARK_BENCHMARK_CPU_H
