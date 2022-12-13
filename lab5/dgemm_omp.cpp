#include <omp.h>
#include <iostream>
#include <chrono>

void matrix_vector_product_omp(double*,double*,double*,int,int);

using namespace std;

int main(int argc, char **argv)
{
    int size = 1000;
    int THREADS = 10;
    double *a, *b, *c;
    a = new double[size * size];
    b = new double[size * size];
    c = new double[size * size];
    cout << "OpenMP:\n";
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            //*(a + i * size + j) = i * size + j;
            //*(b + i * size + j) = j + i;
            *(a + i * size + j) = i;
            *(b + i * size + j) = 1.0;

        }
    }
    auto r_tnow = std::chrono::high_resolution_clock::now();
    matrix_vector_product_omp(a,b,c,size,THREADS);
    auto r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - r_tnow);
    cout << "Thread-num\t" << THREADS << "\tTime\t" << (double)r_time.count() * 1e-9 << "\n";     
    return 0;
}

void matrix_vector_product_omp(double *a, double *b, double *c, int size, int THREADS) {
  // Paralelim
  int i, j, k;
  #pragma omp parallel private(i,j,k) shared(a ,b, c) num_threads(THREADS)
  {
    int threadId = omp_get_thread_num(); // Amount of threads
    int itemsPerThread = size / THREADS;
    int lb = threadId * itemsPerThread;
    int ub = (threadId == THREADS - 1) ? (size) : (lb + itemsPerThread);
    #pragma omp for schedule(static)
    for (i = 0; i < size; ++i) {
      for (k = 0; k < size; ++k) {
        for (j = 0; j < size; ++j) {
          *(c + i * size + j) += *(a + i * size + k) * *(b + k * size + j);
          }
        }
      }
    } 
    //for (int i = 0; i < size * size; i++) {
      //cout << c[i] << " ";   
      //if ((i + 1) % size == 0)
        //cout<<"\n";   
    //}   
}
