//
// Created by hello on 27.09.2022.
//

#include "benchmark_cpu.h"

double meanSquareDeviation(double* array, int n, char combination) {
    double averageSum = 0;
    double difference = 0;
    double sumDifference = 0;
    double dispersion = 0;
    double deviation = 0;

    for (int i = 0; i < n; i++)
        averageSum += array[i];
    averageSum /= n;

    for (int i = 0; i < n; i++) {
        difference = array[i] - averageSum;
        sumDifference += pow(difference, 2);
    }

    if (combination == 'g')
        dispersion = sumDifference / n;
    else if (combination == 'n')
        dispersion = sumDifference / (n - 1);

    deviation = sqrt(dispersion);

    return deviation;
}

string getCpuBrand() {
    std::array<int, 4> integerBuffer = {};
    constexpr size_t sizeofIntegerBuffer = sizeof(int) * integerBuffer.size();
    std::array<char, 64> charBuffer = {};
    constexpr std::array<int, 2> functionIds = {
            static_cast<int>(0x80000002),
            static_cast<int>(0x80000003)
            //static_cast<int>(0x80000004)
    };
    std::string cpu;
    for (int id : functionIds)
    {
        __cpuid(integerBuffer.data(), id);
        memcpy(charBuffer.data(), integerBuffer.data(), sizeofIntegerBuffer);
        cpu += std::string(charBuffer.data());
    }
    return cpu;
}

void matrixVectorProduct(double *a, double *b, double *c, int m, int n)
{
    for (int i = 0; i < m; i++) {
        c[i] = 0.0;
        for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
    }
}

void matrixVectorProduct(int *a, int *b, int *c, int m, int n)
{
    for (int i = 0; i < m; i++) {
        c[i] = 0.0;
        for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
    }
}

void matrixVectorProduct(float *a, float *b, float *c, int m, int n)
{
    for (int i = 0; i < m; i++) {
        c[i] = 0.0;
        for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
    }
}

void printFile(double AvTime, double AbsError, double RelError, double TaskPerf, double time_one) {
    FILE *fp;
    fp = fopen("result.csv", "a");

    char *info;
    info = static_cast<char *>(malloc(sizeof(char) * 512));
    info = "Processor model:\tmodel\nTask:\tMatrix*vector\nOperand Type:\tDouble\nOptimisations:\tNone\nInstruction count:\t30000\nTimer:\tchrono()";
    cout << RelError;
    fprintf(fp, "%s\nTime:\t%f\nLaunch number:\t10\nAverageTime:\t%f\nAbsolute error:\t%f\nRelative error:\t%.16f\nTask performance:\t%.f\n\n", info, time_one, AvTime, AbsError, RelError, TaskPerf);

    fclose(fp);
}

void benchmark_cpu() {
    double *a, *b, *c;
    int *a1, *b1, *c1;
    float *a2, *b2, *c2;
    int size = 10000;
    a = static_cast<double *>(malloc(sizeof(*a) * size * size));
    b = static_cast<double *>(malloc(sizeof(*b) * size));
    c = static_cast<double *>(malloc(sizeof(*c) * size));
    a1 = static_cast<int *>(malloc(sizeof(*a1) * size * size));
    b1 = static_cast<int *>(malloc(sizeof(*b1) * size));
    c1 = static_cast<int *>(malloc(sizeof(*c1) * size));
    a2 = static_cast<float *>(malloc(sizeof(a2) * size * size));
    b2 = static_cast<float *>(malloc(sizeof(b2) * size));
    c2 = static_cast<float *>(malloc(sizeof(c2) * size));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            a[i * size + j] = rand() % 1000;
    }


    string PModel = getCpuBrand();
    string Task = "Matrix multiply";
    string OpType = "long int";
    string Opt = "-j 6";
    li aOp = 30000;

    string Timer = "clock";
    double AvTime = 0;
    double AbsError = 0.0;
    double RealError = 0.0;
    double TaskPerf = 0.0;
    short int amExp = 10;
    double *times = new double[amExp];
    for (int i = 0; i < amExp; ++i) times[i] = 0;

    matrixVectorProduct(a2,b2,c2,size,size);

    for (int i = 0; i < amExp; ++i) {
        auto tnow = std::chrono::high_resolution_clock::now();
        //matrixVectorProduct(a, b, c, size, size);
        //matrixVectorProduct(a1,b1,c1,size,size);
        matrixVectorProduct(a2,b2,c2,size,size);
        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - tnow);
        cout << PModel << " " << Task << " " << OpType << " " << Opt << " " << aOp << " " << Timer << " ";
        std::cout << time.count() << " nsec " << i + 1 << "\n";
        AvTime += time.count();
        times[i] = time.count() * 1e-9;
        TaskPerf += (aOp / times[i]);
    }
    AvTime /= amExp;
    cout << "AvTime = " << AvTime << "\n";
    AbsError = meanSquareDeviation(times, amExp, (char) 'g');
    RealError = AbsError / AvTime * 100;
    cout << "AbsError = " << AbsError << "\n";
    cout << "RealError = " << RealError << "\n";
    cout << "TaskPerf = " << TaskPerf << "\n";
    printFile(AvTime, AbsError, RealError, TaskPerf, times[0]);
    free(a);
    free(times);
    free(b);
    free(c);
    free(a1);
    free(b1);
    free(c1);
    free(a2);
    free(b2);
    free(c2);
}