#pragma once

#include <chrono>
#include <iostream>

class Timer {
public:
	Timer();
	~Timer();
private:
	std::chrono::system_clock::time_point m_start_time;
};


class AveragingTimer {
public:
	AveragingTimer();
	~AveragingTimer();
	static void report_average();
	static void reset();
private:
	std::chrono::system_clock::time_point m_start_time;
    static float instances;
    static float total_elapsed;
};