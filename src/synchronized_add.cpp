#include "synchronized_add.hpp"
#include <iostream>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <mutex>
#include <vector>
#include <chrono>

void synchronizedAdd(int maxVal, unsigned thread_count) {
    std::vector<std::thread> threads;

    std::atomic<int> counter = 0;
    std::mutex mtx;
    std::condition_variable cv;

    auto threadFunc = [&](const int turn) {
        while (counter < maxVal) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&] { return counter % thread_count == turn || counter >= maxVal; });

            if (counter >= maxVal)
                break;

            ++counter;
            cv.notify_all();
        }
    };

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back(threadFunc, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Synchronized add with " << thread_count << " threads: " << counter << " (expected: " << maxVal << ")" << std::endl;
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}

void testSynchronizedAdd(int maxVal, unsigned thread_count) {
    for (int i = 1; i <= thread_count; i *= 2) {
        synchronizedAdd(maxVal, i);
        if (i < thread_count) {
            std::cout << std::endl;
        }
    }
}
