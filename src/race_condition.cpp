#include "race_condition.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

const long long iterations = 100'000'000;

void withoutLock(long long& counter) {
    for (long long i = 0; i < iterations; ++i)
        counter++;
}

void withMutex(long long& counter, std::mutex& mtx) {
    for (long long i = 0; i < iterations; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        counter++;
    }
}

void withAtomic(std::atomic<long long>& counter) {
    for (long long i = 0; i < iterations; ++i)
        counter++;
}

void testRaceCondition() {
    {
        long long counter = 0;
        auto start = std::chrono::high_resolution_clock::now();
        std::thread t1(withoutLock, std::ref(counter));
        std::thread t2(withoutLock, std::ref(counter));
        t1.join(); t2.join();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Race condition result: " << counter << " (expected " << iterations * 2 << ")\n";
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n" << std::endl;
    }

    {
        long long counter = 0;
        std::mutex mtx;
        auto start = std::chrono::high_resolution_clock::now();
        std::thread t1(withMutex, std::ref(counter), std::ref(mtx));
        std::thread t2(withMutex, std::ref(counter), std::ref(mtx));
        t1.join(); t2.join();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "With mutex: " << counter << std::endl;
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n" << std::endl;
    }

    {
        std::atomic<long long> counter(0);
        auto start = std::chrono::high_resolution_clock::now();
        std::thread t1(withAtomic, std::ref(counter));
        std::thread t2(withAtomic, std::ref(counter));
        t1.join(); t2.join();
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "With atomic: " << counter << std::endl;
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms\n" << std::endl;
    }
}

