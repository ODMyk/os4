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

auto create_random_matrix(int n, int m) {
    vector<vector<int>> A(n, vector<int>(m));

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 10);
    for (auto& row : A)
        for (auto& val : row)
            val = dist(gen);

    return A;
}

void matrixMultiply(const vector<vector<int>>& A, const vector<vector<int>>& B, int numThreads) {
    int n = A.size(), m = A[0].size(), k = B[0].size();
    vector<thread> threads;
    vector<vector<int>> result(n, vector<int>(k, 0));
    auto start = high_resolution_clock::now();

    int created = 0;
    for (int i = 0; i < n && created < numThreads; ++i) {
        for (int j = 0; j < k && created < numThreads; ++j) {
            threads.emplace_back(multiplyElement, cref(A), cref(B), ref(result), i, j, m);
            created++;
        }
    }

    for (auto& t : threads) t.join();

    auto end = high_resolution_clock::now();

    cout << "Time: " << duration_cast<microseconds>(end - start).count() << " μs" << endl;
}

void testPerformance(int n, int m, int k, int max_threads) {
    cout << "Generating matrices...\n" << endl;
    auto A = create_random_matrix(n, m);
    auto B = create_random_matrix(m, k);

    cout << "Running tests:" << endl;

    for (int threads = 1; threads <= max_threads; threads *= 2) {
        cout << "\nThreads: " << threads << endl;
        matrixMultiply(A, B, threads);
    }
}

