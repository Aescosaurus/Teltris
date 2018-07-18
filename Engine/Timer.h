#pragma once

class Timer
{
public:
	Timer( float max )
		:
		maxTime( max )
	{}
	void Update( float dt )
	{
		curTime += dt;
	}
	void Reset()
	{
		curTime = 0.0f;
	}
	bool IsDone() const
	{
		return( curTime >= maxTime );
	}
private:
	const float maxTime;
	float curTime = 0.0f;
};