#pragma once

#include <chrono>

class FrameTimer
{
public:
	static float Mark();
private:
	FrameTimer();
	float _Mark();
	static FrameTimer& Get();
private:
	std::chrono::steady_clock::time_point last;
};