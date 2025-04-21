#include "matrix_multiply.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <mutex>
#include <chrono>

using namespace std;
using namespace chrono;

mutex print_mutex;

void multiplyElement(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int i, int j, int m) {
    int sum = 0;
    for (int k = 0; k < m; ++k) {
        sum += A[i][k] * B[k][j];
    }
    C[i][j] = sum;

    lock_guard<mutex> lock(print_mutex);
    cout << "[" << i << "," << j << "] = " << sum << endl;
}

void matrixMultiply(int n, int m, int k, int numThreads) {
    vector<vector<int>> A(n, vector<int>(m));
    vector<vector<int>> B(m, vector<int>(k));
    vector<vector<int>> C(n, vector<int>(k, 0));

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 10);
    for (auto& row : A)
        for (auto& val : row)
            val = dist(gen);
    for (auto& row : B)
        for (auto& val : row)
            val = dist(gen);

    vector<thread> threads;
    auto start = high_resolution_clock::now();

    int created = 0;
    for (int i = 0; i < n && created < numThreads; ++i) {
        for (int j = 0; j < k && created < numThreads; ++j) {
            threads.emplace_back(multiplyElement, cref(A), cref(B), ref(C), i, j, m);
            created++;
        }
    }

    for (auto& t : threads) t.join();

    auto end = high_resolution_clock::now();

    cout << "Time: " << duration_cast<microseconds>(end - start).count() << " μs" << endl;
}

void testPerformance(int n, int m, int k) {
    for (int threads = 1; threads <= 16; threads *= 2) {
        cout << "\nThreads: " << threads << endl;
        matrixMultiply(n, m, k, threads);
    }
}
