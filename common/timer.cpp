#include <chrono>
#include <iostream>

#include "timer.h"

Timer::Timer() 
    : m_start_time(std::chrono::system_clock::now()) {
}
    
Timer::~Timer() {
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedTime = end_time - m_start_time;
    std::cout << "Elapsed time: " << elapsedTime.count() * 1000 << " ms\n";
}



AveragingTimer::AveragingTimer() 
    : m_start_time(std::chrono::system_clock::now()) {
    instances += 1;
}

AveragingTimer::~AveragingTimer() {
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsedTime = end_time - m_start_time;
    total_elapsed += elapsedTime.count();
}

void AveragingTimer::report_average() {
    if (instances == 0.0f) {
        std::cout << "Averaging Timer: Nothing to report.\n";
        return;
    }
    std::cout << "Took " << total_elapsed << " s to run " << instances << " times.\n"
        << "Average: " << total_elapsed / instances * 1000 << " ms\n";
}

void AveragingTimer::reset() {
    instances = 0.0f;
    total_elapsed = 0.0f;
}

float AveragingTimer::instances = 0.0f;
float AveragingTimer::total_elapsed = 0.0f;