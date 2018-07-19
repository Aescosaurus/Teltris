#pragma once

#include "Tetreon.h"
#include "Vec2.h"
#include "Graphics.h"
#include "Timer.h"
#include "Keyboard.h"
#include "Arena.h"
#include <map>

class Tetris1P
{
private:
	typedef unsigned int uint;
public:
	Tetris1P( Graphics& gfx )
		:
		gfx( gfx )
	{
		keys.insert( { VK_LEFT,false } );
		keys.insert( { VK_RIGHT,false } );
		keys.insert( { VK_DOWN,false } );
	}
	void Update( const float dt,const Keyboard& kbd )
	{
		if( kbd.KeyIsPressed( VK_LEFT ) && !keys[VK_LEFT] )
		{
			piece.GetPos().x -= size;
			keys[VK_LEFT] = true;
			CheckForCollision( { 1,0 },false );
		}
		else if( kbd.KeyIsPressed( VK_RIGHT ) && !keys[VK_RIGHT] )
		{
			piece.GetPos().x += size;
			keys[VK_RIGHT] = true;
			CheckForCollision( { -1,0 },false );
		}
		else if( kbd.KeyIsPressed( VK_DOWN ) && !keys[VK_DOWN] )
		{
			Drop();
			keys[VK_DOWN] = true;
		}

		keys[VK_LEFT] = kbd.KeyIsPressed( VK_LEFT );
		keys[VK_RIGHT] = kbd.KeyIsPressed( VK_RIGHT );
		keys[VK_DOWN] = kbd.KeyIsPressed( VK_DOWN );

		dropTimer.Update( dt );
		if( dropTimer.IsDone() )
		{
			Drop();
		}
	}
	void Draw( Graphics& gfx ) const
	{
		DrawMatrix( arena.GetMat(),
			Vei2( 0,0 ),Arena::dim );
		DrawPiece( piece );
		// DrawMatrix( piece.GetMat(),piece.GetPos() );
	}
	void DrawPiece( const Tetreon& piece ) const
	{
		const auto mat = piece.GetMat();
		const auto offset = piece.GetPos();

		DrawMatrix( mat,offset,
			{ Tetreon::dim,Tetreon::dim } );
	}
	void DrawMatrix( const std::vector<uint>& mat,
		const Vei2& offset,const Vei2& dim ) const
	{
		for( int y = 0; y < dim.y; ++y )
		{
			for( int x = 0; x < dim.x; ++x )
			{
				if( mat[y * dim.x + x] != 0 )
				{
					gfx.DrawRect( size * x + offset.x,
						size * y + offset.y,
						size,size,
						Colors::Red );
				}
			}
		}
	}
	void Drop()
	{
		dropTimer.Reset();
		piece.GetPos().y += size;
		
		CheckForCollision( { 0,-1 },true );
	}
	bool CheckForCollision( const Vei2& offset,bool merge )
	{
		if( arena.Collide( piece ) )
		{
			// piece.GetPos().y -= size;
			piece.GetPos() += offset * size;
			if( merge )
			{
				arena.Merge( piece );
				piece.GetPos().y = 0;
			}
			return( true );
		}
		else
		{
			return( false );
		}
	}
private:
	Graphics & gfx;
	Tetreon piece = Tetreon::T( Vei2( 0,0 ) );
	Timer dropTimer = { 1.0f };
	static constexpr int size = Tetreon::size;
	Arena arena;
	std::map<int,bool> keys;
};