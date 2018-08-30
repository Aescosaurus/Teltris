#pragma once

#include <vector>
#include "Tetreon.h"
#include "Graphics.h"
#include "Surface.h"
#include "SpriteEffect.h"

class Arena
{
private:
	typedef unsigned int uint;
public:
	void Draw( const Vei2& offset,Graphics& gfx ) const
	{
		static constexpr int nHidden = 2;
		const int xX = offset.x;
		const int yY = offset.y + nHidden * Tetreon::size;
		const int wW = width * Tetreon::size;
		const int hH = ( height - nHidden ) * Tetreon::size;
		
		gfx.DrawRect( xX,yY,wW,hH,Colors::Gray );

		// for( int y = 0; y < height; ++y )
		// {
		// 	for( int x = 0; x < width; ++x )
		// 	{
		// 		const auto xPos = ( x * size ) + offset.x;
		// 		const auto yPos = ( y * size ) + offset.y;
		// 		// gfx.DrawRect( xPos,
		// 		// 	yPos,
		// 		// 	size,size,
		// 		// 	Colors::Gray );
		// 		gfx.DrawSprite( xPos,yPos,gridTile,
		// 			SpriteEffect::Copy{} );
		// 	}
		// }
	}
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
	int Sweep( int level )
	{
		int nLinesCleared = 0;
		// for( int i = 0; i < 3; ++i )
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
					y = height;
					++nLinesCleared;
				}
			}
		}

		if( nLinesCleared > 4 )
		{
			assert( false );
			nLinesCleared = 2;
		}
		if( nLinesCleared != 0 )
		{
			++nTetrises;
		}
		// Do stuff with nLinesCleared.
		switch( nLinesCleared )
		{
		case 0: return( 0 ); break;
		case 1: return( 40 * ( level + 1 ) ); break;
		case 2: return( 100 * ( level + 1 ) ); break;
		case 3: return( 300 * ( level + 1 ) ); break;
		case 4: return( 1200 * ( level + 1 ) ); break;
		default:
			// You will never get this.
			assert( false );
			break;
		}
		// You *should* never get this either.
		assert( false );
		return( 0 );
	}
	void ShiftDown( int whichLine )
	{
		// If you go top to bottom (like I did like 100
		//  times before realizing it won't work) it will
		//  it will set a line to 0, and the line below to
		//  the one above (all 0), and so on.
		/*
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
		*/
		for( int y = whichLine; y >= 0; --y )
		{
			for( int x = 0; x < width; ++x )
			{
				if( y == 0 )
				{
					const int spot = x;
					data[spot] = 0u;
				}
				else
				{
					const int spot = y * width + x;
					const int spot2 = ( y - 1 ) *
						width + x;

					data[spot] = data[spot2];
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
		auto pos = player.GetPos() / Tetreon::size;

		for( int y = 0; y < player.GetDim(); ++y )
		{
			for( int x = 0; x < player.GetDim(); ++x )
			{
				const auto dataPos = ( y + pos.y ) *
					width + ( x + pos.x );
				const auto amount = y * player.GetDim() + x;

				// Man this should be a little cleaner huh bud?
				//  Anyway, this error messed me up for
				//  a LONG time.  amount > 0 WON'T WORK!!
				if( amount >= 0 &&
					amount < int( mat.size() ) &&
					mat[amount] != 0 &&
					( ( y + pos.y >= height ||
					( dataPos >= 0 &&
					dataPos < int( data.size() ) &&
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
	int GetTetrises() const
	{
		return( nTetrises );
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
	static constexpr int height = 22;
	static constexpr Vei2 dim = { width,height };
private:
	std::vector<uint> data = CreateMatrix( width,height );
	// Represents number of times a line is cleared, but
	//  a 1/2/3/4 line clear counts as the same.
	int nTetrises = 0;
	// static constexpr int size = Tetreon::size;
	// const Surface gridTile = Surface( "Images/GridTile.bmp" )
	// 	.GetExpanded( size,size );
};