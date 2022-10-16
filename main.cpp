#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <valarray>

#include "lab_2/benchmark_cpu.h"

using namespace std;

struct benchDisk {
    string MemoryType;
    long int BlockSize;
    string ElementType;
    long int BufferSize;
    long int LaunchNum;
    string Timer;
    long double WriteTime;
    long double AverageWriteTime;
    long double WriteBandWidth; // (BLOCK_SIZE / AVGWRITETIME) * 10 ^ 6 mb/s
    long double AbsErrorWrite;
    long double RelErrorWrite;
    long double ReadTime;
    long double AverageReadTime;
    long double ReadBandWidth; // (BLOCK_SIZE / AVGREADTIME) * 10 ^ 6 mb/s
    long double AbsErrorRead;
    long double RelErrorRead;
};

template<typename T>
double meanSquareDeviation(vector<T> array, int n, char combination) {
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

void RAM_TEST(int TEST_AMOUNTOFLAUNCHES) {
    vector<long double> w_times;
    vector<long double> r_times;
    benchDisk* TESTS = new benchDisk[TEST_AMOUNTOFLAUNCHES];
    size_t CONTAINER_SIZE = 1024 * 1024 * 8;
    for (long int i = 0; i < TEST_AMOUNTOFLAUNCHES; ++i) {
        vector<int> d(CONTAINER_SIZE / 4, 0);
        TESTS[i].MemoryType = "RAM";
        TESTS[i].BlockSize = CONTAINER_SIZE / 1024;
        TESTS[i].ElementType = "INT";
        TESTS[i].BufferSize = sizeof(d[0]);
        TESTS[i].LaunchNum = i + 1;
        TESTS[i].Timer = "CHRONO";
        //WRITE
        auto w_tnow = std::chrono::high_resolution_clock::now();
        for (size_t k = 0; k < d.size(); ++k) {
            d[k] = 1;
        }
        auto w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - w_tnow);
        TESTS[i].WriteTime = w_time.count() * 1e-9;
        w_times.push_back(w_time.count() * 1e-9);
        long double avg = 0.0;
        for (long int j = 0; j < i; ++j) {
            avg += TESTS[j].WriteTime;
        }
        TESTS[i].AverageWriteTime = avg / TESTS[i].LaunchNum;
        TESTS[i].WriteBandWidth = ((CONTAINER_SIZE / 1024) / TESTS[i].AverageWriteTime);
        TESTS[i].AbsErrorWrite = meanSquareDeviation(w_times, w_times.size(), 'g');
        TESTS[i].RelErrorWrite = TESTS[i].AbsErrorWrite / TESTS[i].AverageWriteTime * 100;
        //READ
        auto r_tnow = std::chrono::high_resolution_clock::now();
        for (size_t k = 0; k < d.size(); ++k) {
            cout << d[k];
        }
        auto r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - w_tnow);
        TESTS[i].ReadTime = r_time.count() * 1e-9;
        r_times.push_back(r_time.count() * 1e-9);
        //cout << TESTS[i].ReadTime << "\n";
        avg = 0.0;
        for (long int j = 0; j < i; ++j) {
            avg += TESTS[i].ReadTime;
        }
        TESTS[i].AverageReadTime = avg / TESTS[i].LaunchNum;
        TESTS[i].ReadBandWidth = ((CONTAINER_SIZE / 1024) / TESTS[i].AverageReadTime);
        TESTS[i].AbsErrorRead = meanSquareDeviation(r_times, r_times.size(), 'g');
        TESTS[i].RelErrorRead = TESTS[i].AbsErrorRead / TESTS[i].AverageReadTime * 100;
    }
    cout << "\n\n\n\n\n";
    for (int i = 0; i < TEST_AMOUNTOFLAUNCHES; ++i) {
        cout << "Rel: " << TESTS[i].RelErrorRead << "  " << TESTS[i].RelErrorWrite << " Abs: " << TESTS[i].AbsErrorRead << " " << TESTS[i].AbsErrorWrite <<"\n";
    }
}

void FLASH_TEST(int TEST_AMOUNTOFLAUNCHES) {
    vector<long double> w_times;
    vector<long double> r_times;
    benchDisk* TESTS = new benchDisk[TEST_AMOUNTOFLAUNCHES];
    size_t CONTAINER_SIZE = 1024 * 8 * 1024 * 4; // 8 MB
    for (long int i = 0; i < TEST_AMOUNTOFLAUNCHES; ++i) {
        vector<int> d(CONTAINER_SIZE / 4, 0);
        TESTS[i].MemoryType = "FLASH";
        TESTS[i].BlockSize = CONTAINER_SIZE / 1024;
        TESTS[i].ElementType = "INT";
        TESTS[i].BufferSize = sizeof(d[0]);
        TESTS[i].LaunchNum = i + 1;
        TESTS[i].Timer = "CHRONO";
        //WRITE
        auto w_tnow = std::chrono::high_resolution_clock::now();
        try {
            ofstream fout("E:/test" + to_string(i) + ".txt");
            for (auto& j: d) {
                fout << j;
            }
            fout.close();
        }
        catch (exception e) {
            printf(e.what());
            exit(-1);
        }
        auto w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - w_tnow);
        TESTS[i].WriteTime = w_time.count() * 1e-9;
        w_times.push_back(w_time.count() * 1e-9);
        //cout << TESTS[i].WriteTime << " sec\n";
        long double avg = 0.0;
        for (long int j = 0; j < i; ++j) {
            avg += TESTS[j].WriteTime;
        }
        TESTS[i].AverageWriteTime = avg / TESTS[i].LaunchNum;
        TESTS[i].WriteBandWidth = ((CONTAINER_SIZE / 1024) / TESTS[i].AverageWriteTime); // *1e6
        TESTS[i].AbsErrorWrite = meanSquareDeviation(w_times, w_times.size(), 'g');
        TESTS[i].RelErrorWrite = TESTS[i].AbsErrorWrite / TESTS[i].AverageWriteTime * 100;
        // READ
        auto r_tnow = std::chrono::high_resolution_clock::now();
        try {
            ifstream fin("test" + to_string(i) + ".txt", ios::app);
            string line;
            while (getline(fin, line)) {
                line.clear();
            }
            fin.close();
        }
        catch (exception e) {
            printf(e.what());
            exit(-1);
        }
        auto r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - w_tnow);
        TESTS[i].ReadTime = r_time.count() * 1e-9;
        r_times.push_back(r_time.count() * 1e-9);
        //cout << TESTS[i].ReadTime << "\n";
        avg = 0.0;
        for (long int j = 0; j < i; ++j) {
            avg += TESTS[i].ReadTime;
        }
        TESTS[i].AverageReadTime = avg / TESTS[i].LaunchNum;
        TESTS[i].ReadBandWidth = ((CONTAINER_SIZE / 1024) / TESTS[i].AverageReadTime);
        TESTS[i].AbsErrorRead = meanSquareDeviation(r_times, r_times.size(), 'g');
        TESTS[i].RelErrorRead = TESTS[i].AbsErrorRead / TESTS[i].AverageReadTime * 100;
        cout << "Rel: " << TESTS[i].RelErrorRead << "  " << TESTS[i].RelErrorWrite << " Abs: " << TESTS[i].AbsErrorRead << " " << TESTS[i].AbsErrorWrite <<"\n";
    }
}

void SSD_TEST(int TEST_AMOUNTOFLAUNCHES) {
    vector<long double> w_times;
    vector<long double> r_times;
    benchDisk* TESTS = new benchDisk[TEST_AMOUNTOFLAUNCHES];
    size_t CONTAINER_SIZE = 1024 * 8 * 1024; // 8 MB
    for (long int i = 0; i < TEST_AMOUNTOFLAUNCHES; ++i) {
        vector<int> d(CONTAINER_SIZE / 4, 0);
        TESTS[i].MemoryType = "SSD";
        TESTS[i].BlockSize = CONTAINER_SIZE / 1024;
        TESTS[i].ElementType = "INT";
        TESTS[i].BufferSize = sizeof(d[0]);
        TESTS[i].LaunchNum = i + 1;
        TESTS[i].Timer = "CHRONO";
        //WRITE
        auto w_tnow = std::chrono::high_resolution_clock::now();
        try {
            ofstream fout("test" + to_string(i) + ".txt");
            for (auto& j: d) {
                fout << j;
            }
            fout.close();
        }
        catch (exception e) {
            printf(e.what());
            exit(-1);
        }
        auto w_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - w_tnow);
        TESTS[i].WriteTime = w_time.count() * 1e-9;
        w_times.push_back(w_time.count() * 1e-9);
        //cout << TESTS[i].WriteTime << " sec\n";
        long double avg = 0.0;
        for (long int j = 0; j < i; ++j) {
            avg += TESTS[j].WriteTime;
        }
        TESTS[i].AverageWriteTime = avg / TESTS[i].LaunchNum;
        TESTS[i].WriteBandWidth = ((CONTAINER_SIZE / 1024) / TESTS[i].AverageWriteTime); // *1e6
        TESTS[i].AbsErrorWrite = meanSquareDeviation(w_times, w_times.size(), 'g');
        TESTS[i].RelErrorWrite = TESTS[i].AbsErrorWrite / TESTS[i].AverageWriteTime * 100;
        // READ
        auto r_tnow = std::chrono::high_resolution_clock::now();
        try {
            ifstream fin("test" + to_string(i) + ".txt", ios::app);
            string line;
            while (getline(fin, line)) {
                line.clear();
            }
            fin.close();
        }
        catch (exception e) {
            printf(e.what());
            exit(-1);
        }
        auto r_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - w_tnow);
        TESTS[i].ReadTime = r_time.count() * 1e-9;
        r_times.push_back(r_time.count() * 1e-9);
        //cout << TESTS[i].ReadTime << "\n";
        avg = 0.0;
        for (long int j = 0; j < i; ++j) {
            avg += TESTS[i].ReadTime;
        }
        TESTS[i].AverageReadTime = avg / TESTS[i].LaunchNum;
        TESTS[i].ReadBandWidth = ((CONTAINER_SIZE / 1024) / TESTS[i].AverageReadTime);
        TESTS[i].AbsErrorRead = meanSquareDeviation(r_times, r_times.size(), 'g');
        TESTS[i].RelErrorRead = TESTS[i].AbsErrorRead / TESTS[i].AverageReadTime * 100;
        cout << "Rel: " << TESTS[i].RelErrorRead << "  " << TESTS[i].RelErrorWrite << " Abs: " << TESTS[i].AbsErrorRead << " " << TESTS[i].AbsErrorWrite <<"\n";
    }
}

int main() {
    std::iostream::sync_with_stdio(false);
    string TEST_TYPE;
    long int TEST_AMOUNTOFLAUNCHES;
    cout << "Enter a params for test like this (without \"[]\"):\n"
            "[Test type(RAM/FLASH/SSD)] [10 (Amount of launches)]\n";
    cin >> TEST_TYPE >> TEST_AMOUNTOFLAUNCHES;
    if (TEST_TYPE == "RAM" || TEST_TYPE == "FLASH" || TEST_TYPE == "SSD") {
        if (TEST_TYPE == "RAM") {
            RAM_TEST(TEST_AMOUNTOFLAUNCHES);
        }
        if (TEST_TYPE == "FLASH") {
            FLASH_TEST(TEST_AMOUNTOFLAUNCHES);
        }
        if (TEST_TYPE == "SSD") {
            SSD_TEST(TEST_AMOUNTOFLAUNCHES);
        }
    }
    else {
        cout << "\nIncorrect arguments"
                "\nCorrect command: ./main \"RAM/SSD/FLASH\" \"Amount of launches\"";
        exit(-1);
    }
}
