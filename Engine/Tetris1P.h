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

#include "Font.h"
#include "Button.h"
#include "Mouse.h"
#include "ScoreSubmitter.h"
#include "HighScoreManager.h"

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
	bool UpdateEnd( Keyboard& kbd,const Mouse& ms,
		HighScoreManager& hsm );
	void Draw( Graphics& gfx ) const;
	void DrawScore( const Vei2& drawPos,
		const Font& f,Graphics& gfx ) const;

	void Reset();
private:
	void DrawPiece( const Tetreon& piece,
		const Vei2& pos ) const;
	void DrawGhostPiece( const Vei2& offset ) const;
	void DrawMatrix( const std::vector<uint>& mat,
		const Vei2& offset,const Vei2& dim ) const;
private:
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
	const Font fixedSys = "Fonts/FixedSys16x28.bmp";
	Graphics& gfx;
	Tetreon piece = Tetreon::T( Vei2( 0,0 ) );
	static constexpr float origDropTime = 1.0f;
	Timer dropTimer = { origDropTime };
	// dropTimeChange needs tweaking to be perfect.
	static constexpr float dropTimeChange = 0.04f;
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

	bool gameOver = false;
	ScoreSubmitter endScore;
};