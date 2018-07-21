#pragma once

#include "Tetreon.h"
#include "Vec2.h"
#include "Graphics.h"
#include "Timer.h"
#include "Keyboard.h"
#include "Arena.h"
#include <map>
#include "Random.h"

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
		keys.insert( { VK_UP,false } );

		ResetPlayer();
	}
	void Update( const float dt,const Keyboard& kbd )
	{
		if( kbd.KeyIsPressed( VK_LEFT ) && !keys[VK_LEFT] )
		{
			keys[VK_LEFT] = true;
			// piece.GetPos().x -= size;
			// CheckForCollision( { 1,0 },false );
			MovePlayer( -1 );
		}
		else if( kbd.KeyIsPressed( VK_RIGHT ) && !keys[VK_RIGHT] )
		{
			keys[VK_RIGHT] = true;
			// piece.GetPos().x += size;
			// CheckForCollision( { -1,0 },false );
			MovePlayer( 1 );
		}
		else if( kbd.KeyIsPressed( VK_DOWN ) && !keys[VK_DOWN] )
		{
			Drop();
			keys[VK_DOWN] = true;
		}
		else if( kbd.KeyIsPressed( VK_UP ) && !keys[VK_UP] )
		{
			RotatePlayer( 1 );
			keys[VK_UP] = true;
		}

		keys[VK_UP] = kbd.KeyIsPressed( VK_UP );
		keys[VK_DOWN] = kbd.KeyIsPressed( VK_DOWN );
		keys[VK_LEFT] = kbd.KeyIsPressed( VK_LEFT );
		keys[VK_RIGHT] = kbd.KeyIsPressed( VK_RIGHT );

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
			{ piece.GetDim(),piece.GetDim() } );
	}
	void DrawMatrix( const std::vector<uint>& mat,
		const Vei2& offset,const Vei2& dim ) const
	{
		for( int y = 0; y < dim.y; ++y )
		{
			for( int x = 0; x < dim.x; ++x )
			{
				const auto spot = y * dim.x + x;
				if( spot < int( mat.size() ) &&
					spot >= 0 &&
					mat[spot] != 0 )
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
		
		// CheckForCollision( { 0,-1 },true );
		if( arena.Collide( piece ) )
		{
			piece.GetPos().y -= size;
			arena.Merge( piece );
			ResetPlayer();
		}
	}
	void MovePlayer( int amount )
	{
		piece.GetPos().x += amount * size;
		if( arena.Collide( piece ) )
		{
			piece.GetPos().x -= amount * size;
		}
	}
	void RotatePlayer( int dir )
	{
		const auto origXPos = piece.GetPos().x;
		piece = Tetreon::Rotate( piece,dir );
		int offset = 1;
		while( arena.Collide( piece ) )
		{
			piece.GetPos().x += offset * size;

			offset = -( offset + ( offset > 0 ? 1 : -1 ) );

			if( offset > piece.GetDim() * 2/*0*/ )
			{
				piece = Tetreon::Rotate( piece,-dir );
				piece.GetPos().x = origXPos;
				break;
			}
		}
	}
	void ResetPlayer()
	{
		const int rnd = Random::RangeI( 0,6 );
		switch( rnd )
		{
		case 0: piece = Tetreon::T(); break;
		case 1: piece = Tetreon::O(); break;
		case 2: piece = Tetreon::L(); break;
		case 3: piece = Tetreon::J(); break;
		case 4: piece = Tetreon::I(); break;
		case 5: piece = Tetreon::S(); break;
		case 6: piece = Tetreon::Z(); break;
		default:
			assert( false );
			break;
		}

		piece.GetPos().x = Arena::width / 2 * size;
		piece.GetPos().y = 0;
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
	Graphics& gfx;
	Tetreon piece = Tetreon::T( Vei2( 0,0 ) );
	Timer dropTimer = { 1.0f };
	static constexpr int size = Tetreon::size;
	Arena arena;
	std::map<int,bool> keys;
};