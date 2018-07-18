#pragma once

#include <random>

class Random
{
public:
	static int Range( int min,int max );
	static float Range( float min,float max );
	static int RangeI( int min,int max );
	static float RangeF( float min,float max );
	static int NextInt( int min,int max );
	static float NextFloat( float min,float max );
private:
	Random();
	static Random& Get();
	int _NextInt( int min,int max ) const;
	float _NextFloat( float min,float max ) const;
private:
	mutable std::mt19937 rng;
};