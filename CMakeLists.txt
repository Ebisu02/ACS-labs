cmake_minimum_required(VERSION 3.22.3)
project(benchmark)

set(CMAKE_CXX_STANDARD 20)

add_executable(benchmark main.cpp lab_2/benchmark_cpu.cpp lab_2/benchmark_cpu.h lab_3/memory_bench.cpp lab_3/memory_bench.h lab_4/multiple_matrix.cpp lab_4/multiple_matrix.h)

