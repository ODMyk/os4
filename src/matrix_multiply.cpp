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

void multiplyElement(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int i, int j, int m, bool print = false) {
    int sum = 0;
    for (int k = 0; k < m; ++k) {
        sum += A[i][k] * B[k][j];
    }
    C[i][j] = sum;

    if (!print)
        return;

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

void matrixMultiply(const vector<vector<int>>& A, const vector<vector<int>>& B, int numThreads, bool print = false) {
    int n = A.size(), m = A[0].size(), k = B[0].size();
    vector<thread> threads;
    vector<vector<int>> result(n, vector<int>(k, 0));
    auto start = high_resolution_clock::now();

    // Create threads to multiply elements of result matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            // Start a new thread for each element (i, j)
            threads.emplace_back(multiplyElement, cref(A), cref(B), ref(result), i, j, m, print);
            // If number of threads exceeds numThreads, wait for some to finish
            if (threads.size() >= numThreads) {
                for (auto& t : threads) {
                    t.join();  // Join threads before creating new ones
                }
                threads.clear();  // Clear threads for new ones to be created
            }
        }
    }

    // Join any remaining threads
    for (auto& t : threads) {
        t.join();
    }

    auto end = high_resolution_clock::now();

    cout << "Time: " << duration_cast<milliseconds>(end - start).count() << "ms" << endl;
}

void testPerformance(int n, int m, int k, int max_threads) {
    auto A = create_random_matrix(n, m);
    auto B = create_random_matrix(m, k);

    for (int threads = 1; threads <= max_threads; threads *= 2) {
        cout << "\nThreads: " << threads << endl;
        matrixMultiply(A, B, threads);
    }
}

void basicTest() {
    vector<vector<int>> A(4, vector<int>(4));
    vector<vector<int>> B(4, vector<int>(4));
    vector<vector<int>> C(4, vector<int>(4));

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            A[i][j] = i + j;
            B[i][j] = (i + 1) * (j + 1);
        }
    }

    matrixMultiply(A, B, 16, true);
}