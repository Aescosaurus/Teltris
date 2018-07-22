#pragma once

#include <vector>
#include "Tetreon.h"
#include "Graphics.h"

class Arena
{
private:
	typedef unsigned int uint;
public:
	void Merge( const Tetreon& piece )
	{
		const auto& mat = piece.GetMat();
		const auto& pos = piece.GetPos() / Tetreon::size;

		for( int y = 0; y < piece.GetDim(); ++y )
		{
			for( int x = 0; x < piece.GetDim(); ++x )
			{
				const uint info = mat[y * piece.GetDim() + x];
				if( info != 0 )
				{
					data[( y + pos.y ) * width +
						x + pos.x] = info;
				}
			}
		}
	}
	void Clear()
	{
		data = CreateMatrix( width,height );
	}
	void Sweep()
	{
		for( int i = 0; i < 2; ++i )
		{
			for( int y = height - 1; y > 0; --y )
			{
				bool willReplace = true;
				for( int x = 0; x < width; ++x )
				{
					if( data[y * width + x] == 0 )
					{
						willReplace = false;
						continue;
					}
				}

				if( willReplace )
				{
					ShiftDown( y );
				}
			}
		}
	}
	void ShiftDown( int whichLine )
	{

		// If you go top to bottom (like I did like 100
		//  times before realizing it won't work) it will
		//  it will set a line to 0, and the line below to
		//  the one above (all 0), and so on.
		for( int i = width * height - 1; i >= 0; --i )
		{
			const auto spot = i - width;
			if( spot >= 0 )
			{
				data[i] = data[spot];
			}
			else
			{
				data[i] = 0;
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
private:
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
public:
	static constexpr int width = 10;
	static constexpr int height = 20;
	static constexpr Vei2 dim = { width,height };
private:
	std::vector<uint> data = CreateMatrix( width,height );
};