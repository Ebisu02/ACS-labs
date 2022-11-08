#include "lab_2/benchmark_cpu.h"
#include "lab_3/memory_bench.h"

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <chrono>

//double A[N * N], B[N * N], C[N * N];

void multi_matrix(double *a, double *b, double *c, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                *(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
                //c[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}

void init_matrix_mod(double *c, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                *(c + i * n + j) = 0.0;
            }
        }
    }
}

void matrix_multi_mod(double *a, double *b, double *c, int n)
{
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                *(c + i * n + j) += *(a + i * n + k) * *(b + k * n + j);
            }
        }
    }
}

void init_matrix(double *a, double *b, double *c, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                *(a + i * n + j) = rand() % 100;
                *(b + i * n + j) = rand() % 100;
                *(c + i * n + j) = 0.0;
            }
        }
    }
}

void print_matrix(double *a, int n)
{
    printf("Matrix:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%12.2f", *(a + i * n + j));
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    int init_re = 0;
    for (int i = 0; i < 1000; ++i) {
        init_re += init_re * i;
    }
    srand(time(0));
    int N = 1000;
    double *A, *B, *C;
    A = static_cast<double *>(malloc(N * N * sizeof(double)));
    B = static_cast<double *>(malloc(N * N * sizeof(double)));
    C = static_cast<double *>(malloc(N * N * sizeof(double)));
    if(argv[1] != NULL) {
        N = atoi(argv[1]);
    }
    auto tnow = std::chrono::high_resolution_clock::now();
    init_matrix(A, B, C, N);
    multi_matrix(A, B, C, N);
    auto tafter = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - tnow);
    auto time = tafter.count() * 1e-9;
    printf("Elapsed time for matrix_multi_default:   %.7f sec.\n", time);

    double *D, *E, *F;
    D = static_cast<double *>(malloc(N * N * sizeof(double)));
    E = static_cast<double *>(malloc(N * N * sizeof(double)));
    F = static_cast<double *>(malloc(N * N * sizeof(double)));
    auto tnow_mod = std::chrono::high_resolution_clock::now();
    init_matrix_mod(F, N);
    matrix_multi_mod(E, D, F, N);
    auto tafter_mod = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - tnow_mod);
    printf("Elapsed time for matrix_multi_mod: %.7f sec.\n", tafter_mod.count() * 1e-9);
    FILE *fp;
    fp = fopen("result.csv", "a");
   // fprintf(fp, "Default\t%i\t%lf\n", N, tafter.count() * 1e-9);
   // fprintf(fp, "Mod\t%i\t%lf\n", N, tafter_mod.count() * 1e-9);
}
