#include "Random.h"

Random::Random()
	:
	rng( std::random_device{}( ) )
{
}

int Random::Range( int min,int max )
{
	return RangeI( min,max );
}

float Random::Range( float min,float max )
{
	return RangeF( min,max );
}

int Random::RangeI( int min,int max )
{
	return( Get()._NextInt( min,max ) );
}

float Random::RangeF( float min,float max )
{
	return( Get()._NextFloat( min,max ) );
}

int Random::NextInt( int min,int max )
{
	return RangeI( min,max );
}

float Random::NextFloat( float min,float max )
{
	return RangeF( min,max );
}

Random& Random::Get()
{
	static Random rng;
	return( rng );
}

int Random::_NextInt( int min,int max ) const
{
	std::uniform_int_distribution<int> dist( min,max );
	return dist( rng );
}

float Random::_NextFloat( float min,float max ) const
{
	std::uniform_real_distribution<float> dist( min,max );
	return dist( rng );
}