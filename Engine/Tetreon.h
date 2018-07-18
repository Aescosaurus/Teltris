#pragma once

#include <vector>
#include <cassert>

class Tetreon
{
private:
	typedef unsigned int uint;
public:
	Tetreon( const std::vector<uint>& data,
		const Vei2& pos )
		:
		pos( pos )
	{
		assert( data.size() == 9 );
		matrix.reserve( 9 );
		for( int i = 0; i < 9; ++i )
		{
			matrix.emplace_back( data[i] );
		}
	}
	Tetreon( const std::vector<uint>& data )
		:
		Tetreon( data,Vei2( 0,0 ) )
	{}
	const std::vector<uint>& GetMat() const
	{
		return( matrix );
	}
	const Vei2& GetPos() const
	{
		return( pos );
	}
	Vei2& GetPos()
	{
		return( pos );
	}
	static Tetreon T( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,0,0,
			1,1,1,
			0,1,0 },pos ) );
	}
public:
	static constexpr int dim = 3;
	static constexpr int size = 20;
private:
	std::vector<uint> matrix;
	Vei2 pos;
};