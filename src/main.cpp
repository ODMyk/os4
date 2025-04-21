#include <iostream>

#include "matrix_multiply.hpp"
#include "race_condition.hpp"
#include "synchronized_add.hpp"

#define N 400
#define M 600
#define K 400

#define SYNC_VALUE 1000000

int main() {
    std::cout << "\n--- 1. Matrix Multiplication with Parallelism ---\n";
    basicTest();
    std::cout << "\n" << N << " x " << M << " x " << K << " Test:\n";
    testPerformance(N, M, K, 8192);

    std::cout << "\n--- 2. Race Condition Test ---\n";
    testRaceCondition();

    std::cout << "\n--- 2* Synchronized Parallel Add ---\n";
    testSynchronizedAdd(SYNC_VALUE, 16);

    return 0;
}
