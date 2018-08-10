#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Timer.h"
#include "Keyboard.h"
#include <map>
#include <vector>
#include "Surface.h"
// idk why but including Tetreon and Arena before the other
//  stuff causes lots of errors.. :(
#include "Tetreon.h"
#include "Arena.h"

class Tetris1P
{
private:
	typedef unsigned int uint;
public:
	// Controls: left, right, rotate, drop, full drop, store.
	Tetris1P( uint seed,const Vei2& drawPos,
		const std::vector<int>& controls,
		Graphics& gfx );

	void Update( const float dt,const Keyboard& kbd );
	void Draw( Graphics& gfx ) const;

	void DrawPiece( const Tetreon& piece,
		const Vei2& pos ) const;
	void DrawGhostPiece( const Vei2& offset ) const;
	void DrawMatrix( const std::vector<uint>& mat,
		const Vei2& offset,const Vei2& dim ) const;

	bool Drop();
	void UpdateGhost();
	void MovePlayer( int amount );
	void RotatePlayer( int dir );
	void ResetPlayer();
	void ResetPlayerPos();
	void ShiftNextPieces();
	void SwapStored();
	void RandomizeBag();
	Tetreon::Type GetNextBagItem();
private:
	Graphics& gfx;
	Tetreon piece = Tetreon::T( Vei2( 0,0 ) );
	Timer dropTimer = { 1.0f };
	static constexpr int size = Tetreon::size;
	Arena arena;
	std::map<int,bool> keys;
	Timer downTimer = { 0.067f };

	Tetreon storedPiece = Tetreon::Blank();
	static constexpr int nNextPieces = 3;
	Tetreon nextPieces[nNextPieces];
	std::vector<Surface> pieceIcons;

	static constexpr int bagSize = Tetreon::nPieceTypes;
	std::vector<Tetreon::Type> bag;
	int curBagSpot = 0;
	std::mt19937 rngEng;

	Tetreon ghostPiece = Tetreon::Blank();
	const Surface ghostIcon = Surface( "Images/GhostTile.bmp" )
		.GetExpanded( size,size );

	int curLevel = 0;
	int score = 0;

	const Vei2 start;
	const int left;
	const int right;
	const int rotate;
	const int down;
	const int fDown;
	const int store;

	bool waitedToDrop = false;
	bool canManuallyDrop = false;
};