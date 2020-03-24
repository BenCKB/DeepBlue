#pragma once
#include <chrono>
#include <string>

class Timer {
public:
	void start() {
		isRunning = true;
		startTime = std::chrono::high_resolution_clock::now();
	}

	void stop() {
		endTime = std::chrono::high_resolution_clock::now();
		isRunning = false;
		delta = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch() -
			std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch();
	}

	float getDelta() {
		return (float)delta.count();
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
	bool isRunning = false;

	std::chrono::microseconds delta;

};