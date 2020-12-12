#include <chrono>
#include <iostream>

class Timer {
public:
	Timer() 
		: m_start_time(std::chrono::system_clock::now()) {
	}
	~Timer() {
		auto end_time = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = end_time - m_start_time;
		std::cout << "Elapsed time: " << elapsedTime.count() * 1000 << " ms\n";
	}
private:
	std::chrono::system_clock::time_point m_start_time;
};

// This is very much designed to quickly benchmark these advent of code problems. There's better ways this could be designed,
// but this is fast-to-write, simple, and most importantly, good enough for what I'm doing with it.
class AveragingTimer {
public:
	AveragingTimer() 
		: m_start_time(std::chrono::system_clock::now()) {
		instances += 1;
	}
	~AveragingTimer() {
		auto end_time = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = end_time - m_start_time;
		total_elapsed += elapsedTime.count();
	}
	static void report_average() {
		if (instances == 0.0f) {
			std::cout << "Averaging Timer: Nothing to report.\n";
			return;
		}
		std::cout << "Took " << total_elapsed << " s to run " << instances << " times.\n"
			<< "Average: " << total_elapsed / instances * 1000 << " ms\n";
	}
	static void reset() {
		instances = 0.0f;
		total_elapsed = 0.0f;
	}
private:
	std::chrono::system_clock::time_point m_start_time;
    static float instances;
    static float total_elapsed;
};

float AveragingTimer::instances = 0.0f;
float AveragingTimer::total_elapsed = 0.0f;