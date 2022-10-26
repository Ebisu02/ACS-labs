//
// Created by hello on 26.10.2022.
//

#ifndef BENCHMARK_MEMORY_BENCH_H
#define BENCHMARK_MEMORY_BENCH_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <valarray>

using namespace std;

struct benchDisk {
    string MemoryType;
    long int BlockSize;
    string ElementType;
    long int BufferSize;
    long int LaunchNum;
    string Timer;
    double WriteTime;
    double AverageWriteTime;
    double WriteBandWidth; // (BLOCK_SIZE / AVGWRITETIME) * 10 ^ 6 mb/s
    double AbsErrorWrite;
    double RelErrorWrite;
    double ReadTime;
    double AverageReadTime;
    double ReadBandWidth; // (BLOCK_SIZE / AVGREADTIME) * 10 ^ 6 mb/s
    double AbsErrorRead;
    double RelErrorRead;
};

void start_memory_bench();

#endif //BENCHMARK_MEMORY_BENCH_H
