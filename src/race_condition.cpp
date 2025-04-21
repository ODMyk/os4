#include "race_condition.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

#define ITERATIONS 100000000

void withoutLock(long long& counter) {
    for (long long i = 0; i < ITERATIONS; ++i)
        counter++;
}

void withMutex(long long& counter, std::mutex& mtx) {
    for (long long i = 0; i < ITERATIONS; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        counter++;
    }
}

void withAtomic(std::atomic<long long>& counter) {
    for (long long i = 0; i < ITERATIONS; ++i)
        counter++;
}

template<typename TestFunction>
auto runTest(TestFunction testFunc, const char* testName) {
    long long counter = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    std::thread thread1(testFunc, std::ref(counter));
    std::thread thread2(testFunc, std::ref(counter));
    thread1.join();
    thread2.join();

    auto endTime = std::chrono::high_resolution_clock::now();

    return std::make_tuple(counter, std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count(), testName);
}

void testRaceCondition() {
    constexpr auto expectedCount = ITERATIONS * 2;

    // Test without synchronization
    {
        long long counter = 0;
        auto startTime = std::chrono::high_resolution_clock::now();

        std::thread thread1(withoutLock, std::ref(counter));
        std::thread thread2(withoutLock, std::ref(counter));
        thread1.join();
        thread2.join();

        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "Race condition result: " << counter << " (expected " << expectedCount << ")\n";
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms\n\n";
    }

    // Test with mutex
    {
        long long counter = 0;
        std::mutex counterMutex;
        auto startTime = std::chrono::high_resolution_clock::now();

        std::thread thread1(withMutex, std::ref(counter), std::ref(counterMutex));
        std::thread thread2(withMutex, std::ref(counter), std::ref(counterMutex));
        thread1.join();
        thread2.join();

        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "With mutex: " << counter << "\n";
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms\n\n";
    }

    // Test with atomic
    {
        std::atomic<long long> counter(0);
        auto startTime = std::chrono::high_resolution_clock::now();

        std::thread thread1(withAtomic, std::ref(counter));
        std::thread thread2(withAtomic, std::ref(counter));
        thread1.join();
        thread2.join();

        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "With atomic: " << counter << "\n";
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << " ms\n\n";
    }
}

