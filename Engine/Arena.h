#pragma once

#include <vector>
#include "Tetreon.h"
#include "Graphics.h"

class Arena
{
private:
	typedef unsigned int uint;
public:
	std::vector<uint> CreateMatrix( int w,int h ) const
	{
		std::vector<uint> temp;
		temp.reserve( w * h );
		for( int i = 0; i < w * h; ++i )
		{
			temp.emplace_back( 0 );
		}
		return( temp );
	}
	void Merge( const Tetreon& piece )
	{
		const auto& mat = piece.GetMat();
		const auto& pos = piece.GetPos() / Tetreon::size;

		for( int y = 0; y < piece.GetDim(); ++y )
		{
			for( int x = 0; x < piece.GetDim(); ++x )
			{
				if( mat[y * piece.GetDim() + x] != 0 )
				{
					data[( y + pos.y ) * width +
						x + pos.x] = 1;
				}
			}
		}
	}
	const std::vector<uint>& GetMat() const
	{
		return( data );
	}
	bool Collide( const Tetreon& player ) const
	{
		const auto& mat = player.GetMat();
		const auto& pos = player.GetPos() / Tetreon::size;

		for( int y = 0; y < player.GetDim(); ++y )
		{
			for( int x = 0; x < player.GetDim(); ++x )
			{
				const auto dataPos = ( y + pos.y ) *
					width + ( x + pos.x );
				const auto amount = y * player.GetDim() + x;

				// Man this should be a little cleaner huh bud?
				if( amount > 0 &&
					amount < int( mat.size() ) &&
					mat[amount] != 0 &&
					( ( y + pos.y >= height ||
					( dataPos < int( data.size() ) &&
					data[dataPos] != 0 ) ) ||
					( x + pos.x >= width ||
					x + pos.x < 0 ) ) )
				{
					auto b = mat[y * player.GetDim() + x];
					return( true );
				}
			}
		}
		return( false );
	}
public:
	static constexpr int width = 10;
	static constexpr int height = 20;
	static constexpr Vei2 dim = { width,height };
private:
	std::vector<uint> data = CreateMatrix( width,height );
};