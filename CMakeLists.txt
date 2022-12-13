cmake_minimum_required(VERSION 3.22.3)
project(benchmark)

set(CMAKE_CXX_STANDARD 20)

add_executable(benchmark main.cpp lab_4/multiple_matrix.cpp lab_4/multiple_matrix.h)

