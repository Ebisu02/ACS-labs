g++ dgemm_omp.cpp -o ompbench -fopenmp 
./ompbench
rm ompbench

g++ posix.cpp -o posixbench
./posixbench
