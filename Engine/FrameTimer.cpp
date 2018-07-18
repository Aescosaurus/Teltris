#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer()
{
	last = steady_clock::now();
}

float FrameTimer::Mark()
{
	return( Get()._Mark() );
}

float FrameTimer::_Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

FrameTimer& FrameTimer::Get()
{
	static FrameTimer ft;
	return ft;
}

