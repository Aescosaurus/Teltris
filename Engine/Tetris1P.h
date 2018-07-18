#pragma once

#include "Tetreon.h"
#include "Vec2.h"
#include "Graphics.h"
#include "Timer.h"
#include "Keyboard.h"
#include "Arena.h"

class Tetris1P
{
public:
	Tetris1P( Graphics& gfx )
		:
		gfx( gfx )
	{}
	void Update( const float dt,const Keyboard& kbd )
	{
		if( kbd.KeyIsPressed( VK_LEFT ) )
		{
			piece.GetPos().x -= size;
		}
		else if( kbd.KeyIsPressed( VK_RIGHT ) )
		{
			piece.GetPos().x += size;
		}
		else if( kbd.KeyIsPressed( VK_DOWN ) )
		{
			Drop();
		}

		dropTimer.Update( dt );
		if( dropTimer.IsDone() )
		{
			Drop();
		}
	}
	void Draw( Graphics& gfx ) const
	{
		DrawPiece( piece );
	}
	void DrawPiece( const Tetreon& piece ) const
	{
		const auto mat = piece.GetMat();
		const auto offset = piece.GetPos();

		for( int y = 0; y < Tetreon::dim; ++y )
		{
			for( int x = 0; x < Tetreon::dim; ++x )
			{
				if( mat[y * Tetreon::dim + x] == 1 )
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

		if( arena.Collide( piece ) )
		{
			piece.GetPos().y -= size;
			arena.Merge( piece );
			piece.GetPos().y = 5;
		}
	}
private:
	Graphics & gfx;
	Tetreon piece = Tetreon::T( Vei2( 5,5 ) );
	Timer dropTimer = { 1.0f };
	static constexpr int size = Tetreon::size;
	Arena arena;
};