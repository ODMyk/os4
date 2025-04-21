#include <iostream>

#include "matrix_multiply.hpp"
#include "race_condition.hpp"
#include "synchronized_add.hpp"

constexpr int N = 10000;
constexpr int M = 10000;
constexpr int K = 10000;
constexpr int SYNC_VALUE = 1000000;

int main() {
    // std::cout << "\n--- 1. Matrix Multiplication with Parallelism ---\n";
    // matrixMultiply(N, M, K, 16); // 1.1

    // std::cout << "\n--- 1.2 Performance Test ---\n";
    // testPerformance(N, M, K);

    // std::cout << "\n--- 2. Race Condition Test ---\n";
    // testRaceCondition();

    std::cout << "\n--- 2.3.* Synchronized Parallel Add ---\n";
    synchronizedAdd(SYNC_VALUE, 1);
    synchronizedAdd(SYNC_VALUE, 2);
    synchronizedAdd(SYNC_VALUE, 4);
    synchronizedAdd(SYNC_VALUE, 8);
    synchronizedAdd(SYNC_VALUE, 16);

    return 0;
}
