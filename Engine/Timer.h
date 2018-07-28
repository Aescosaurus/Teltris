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
	void ChangeMax( float amount )
	{
		maxTime += amount;
	}
	bool IsDone() const
	{
		return( curTime >= maxTime );
	}
private:
	float maxTime;
	float curTime = 0.0f;
};