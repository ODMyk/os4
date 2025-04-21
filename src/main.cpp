#include <iostream>

#include "matrix_multiply.hpp"
#include "race_condition.hpp"
#include "synchronized_add.hpp"

constexpr int N = 10000;
constexpr int M = 10000;
constexpr int K = 10000;
constexpr int SYNC_VALUE = 1000000;

int main() {
    std::cout << "\n--- 1. Matrix Multiplication with Parallelism ---\n";
    testPerformance(N, M, K, 16);

    std::cout << "\n--- 2. Race Condition Test ---\n";
    testRaceCondition();

    std::cout << "\n--- 2* Synchronized Parallel Add ---\n";
    testSynchronizedAdd(SYNC_VALUE, 16);

    return 0;
}
