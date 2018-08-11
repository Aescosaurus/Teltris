#include "Tetris1P.h"
#include <algorithm>
#include "SpriteEffect.h"
#include "Random.h"
#include <cassert>

Tetris1P::Tetris1P( uint seed,const Vei2& drawPos,
	const std::vector<int>& controls,
	Graphics& gfx )
	:
	gfx( gfx ),
	rngEng( seed ),
	start( drawPos ),
	left( controls[0] ),
	right( controls[1] ),
	rotate( controls[2] ),
	down( controls[3] ),
	fDown( controls[4] ),
	store( controls[5] )
{
	assert( controls.size() == 6 );

	bag.reserve( bagSize );
	for( int i = 0; i < bagSize; ++i )
	{
		bag.emplace_back( Tetreon::Type::Fake );
	}
	RandomizeBag();

	keys.insert( { left,false } );
	keys.insert( { right,false } );
	keys.insert( { down,false } );
	keys.insert( { rotate,false } );
	keys.insert( { store,false } );
	keys.insert( { fDown,false } );

	for( int i = 0; i < nNextPieces; ++i )
	{
		// nextPieces[i] = Tetreon::GetRandPiece();
		nextPieces[i] = Tetreon::GetPiece( GetNextBagItem() );
	}

	ResetPlayer();

	const Surface base = "Images/Tetreons.bmp";
	pieceIcons.emplace_back( Surface( base,
		RectI( { 8 * 3,8 * 1 },8,8 ) )
		.GetExpanded( size,size ) );
	for( int y = 0; y < 2; ++y )
	{
		for( int x = 0; x < 4; ++x )
		{
			pieceIcons.emplace_back( Surface( base,
				RectI( { 8 * x,8 * y },8,8 ) )
				.GetExpanded( size,size ) );
		}
	}
}

void Tetris1P::Update( const float dt,const Keyboard& kbd )
{
	if( gameOver ) return;

	if( kbd.KeyIsPressed( down ) )
	{
		downTimer.Update( dt );
	}
	else
	{
		downTimer.Reset();
	}
	// 
	if( kbd.KeyIsPressed( left ) && !keys[left] )
	{
		keys[left] = true;
		// piece.GetPos().x -= size;
		// CheckForCollision( { 1,0 },false );
		MovePlayer( -1 );
	}
	else if( kbd.KeyIsPressed( right ) && !keys[right] )
	{
		keys[right] = true;
		// piece.GetPos().x += size;
		// CheckForCollision( { -1,0 },false );
		MovePlayer( 1 );
	}
	else if( kbd.KeyIsPressed( down ) &&
		canManuallyDrop &&
		( !keys[down] || downTimer.IsDone() ) )
	{
		downTimer.Reset();
		if( Drop() )
		{
			canManuallyDrop = false;
		}
		dropTimer.Reset();
		keys[down] = true;
	}
	else if( kbd.KeyIsPressed( rotate ) && !keys[rotate] )
	{
		RotatePlayer( 1 );
		keys[rotate] = true;
	}
	else if( kbd.KeyIsPressed( store ) &&
		!keys[store] )
	{
		SwapStored();
		keys[store] = true;
	}
	else if( kbd.KeyIsPressed( fDown ) &&
		!keys[fDown] )
	{
		// This is kinda cool, I think it works but
		//  will run 1 too many times.  That's ok.
		while( !Drop() );
		dropTimer.Reset();
		downTimer.Reset();
	}

	keys[rotate] = kbd.KeyIsPressed( rotate );
	keys[down] = kbd.KeyIsPressed( down );
	keys[left] = kbd.KeyIsPressed( left );
	keys[right] = kbd.KeyIsPressed( right );
	keys[store] = kbd.KeyIsPressed( store );
	keys[fDown] = kbd.KeyIsPressed( fDown );

	dropTimer.Update( dt );
	if( dropTimer.IsDone() )
	{
		Drop();
	}
}

bool Tetris1P::UpdateEnd( Keyboard& kbd,const Mouse& ms,
	HighScoreManager& hsm )
{
	if( gameOver )
	{
		endScore.Update( kbd,ms );

		if( endScore.IsDone() )
		{
			hsm.AddScore( score,endScore.GetName() );
			return( true );
		}
	}
	return( false );
}

void Tetris1P::Draw( Graphics& gfx ) const
{
	// const Vei2 start = Vei2( 150,50 );
	const Vei2 storedStart = start +
		Vei2{ Arena::width * size,0 } +Vei2( 5 );
	static constexpr Vei2 nextPieceAdd = Vei2( 0,5 );

	arena.Draw( start,gfx );

	DrawMatrix( arena.GetMat(),
		start,Arena::dim );

	DrawPiece( piece,start );
	DrawGhostPiece( start );

	DrawMatrix( storedPiece.GetMat(),
		storedStart,storedPiece.GetDim() );

	// for( const auto& t : nextPieces )
	for( int i = 0; i < nNextPieces; ++i )
	{
		const auto& next = nextPieces[i];
		// Use ( i + 1 ) instead of i to make room
		//  for stored piece.
		const auto myPos = storedStart +
			nextPieceAdd * ( i + 1 ) +
			Vei2( 0,Tetreon::dimL * size * ( i + 1 ) );

		DrawMatrix( next.GetMat(),myPos,
			next.GetDim() );
	}

	if( gameOver )
	{
		endScore.Draw( gfx );
	}
}

void Tetris1P::DrawScore( const Vei2& drawPos,
	const Font& f,Graphics& gfx ) const
{
	f.DrawText( "Score: " + std::to_string( score ),
		drawPos,Colors::White,gfx );
}

void Tetris1P::Reset()
{
	ResetPlayer();
	RandomizeBag();
	storedPiece = Tetreon::Blank();
	curLevel = 0;
	score = 0;
	gameOver = false;
	endScore = ScoreSubmitter{};
	arena.Clear();
}

void Tetris1P::DrawPiece( const Tetreon& piece,
	const Vei2& pos ) const
{
	const auto& mat = piece.GetMat();
	const auto& offset = piece.GetPos() + pos;

	DrawMatrix( mat,offset,
		{ piece.GetDim(),piece.GetDim() } );
}

void Tetris1P::DrawGhostPiece( const Vei2& offset ) const
{
	const auto& mat = ghostPiece.GetMat();
	const auto dim = Vei2( ghostPiece.GetDim() );

	const auto xAdd = offset.x + ghostPiece.GetPos().x;
	const auto yAdd = offset.y + ghostPiece.GetPos().y;
	for( int y = 0; y < dim.y; ++y )
	{
		for( int x = 0; x < dim.x; ++x )
		{
			const auto spot = y * dim.x + x;
			if( spot < int( mat.size() ) &&
				spot >= 0 &&
				mat[spot] != 0 )
			{
				const auto xPos = size * x + xAdd;
				const auto yPos = size * y + yAdd;

				gfx.DrawSprite( xPos,yPos,
					ghostIcon,
					SpriteEffect
					::Chroma{ Colors::Magenta } );
			}
		}
	}
}

void Tetris1P::DrawMatrix( const std::vector<uint>& mat,
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
				// TODO: Replace with images someday.
				// gfx.DrawRect( size * x + offset.x,
				// 	size * y + offset.y,
				// 	size,size,
				// 	Tetreon::tetCols[mat[spot]] );
				gfx.DrawSprite( size * x + offset.x,
					size * y + offset.y,
					pieceIcons[mat[spot]],
					SpriteEffect::Copy{} );
			}
		}
	}
}

bool Tetris1P::Drop()
{
	dropTimer.Reset();
	piece.GetPos().y += size;

	canManuallyDrop = true;

	// CheckForCollision( { 0,-1 },true );
	if( arena.Collide( piece ) )
	{
		if( waitedToDrop )
		{
			waitedToDrop = false;

			piece.GetPos().y -= size;
			arena.Merge( piece );
			ResetPlayer();
			score += arena.Sweep( curLevel );
			// UpdateGhost();
			return( true );
		}
		else
		{
			waitedToDrop = true;
			piece.GetPos().y -= size;
			return( true );
		}
	}
	else
	{
		// UpdateGhost();
		waitedToDrop = false;
		return( false );
	}
}

void Tetris1P::UpdateGhost()
{
	ghostPiece = piece;

	bool done = false;
	while( !done )
	{
		ghostPiece.GetPos().y += size;

		if( arena.Collide( ghostPiece ) )
		{
			ghostPiece.GetPos().y -= size;
			done = true;
		}
	}
}

void Tetris1P::MovePlayer( int amount )
{
	Vei2& pos = piece.GetPos();
	// const auto spot = ( pos.x + amount * size ) / size;
	// if( spot < 0 || spot > Arena::width )
	// {
	// 	return;
	// }
	pos.x += amount * size;
	if( arena.Collide( piece ) )
	{
		pos.x -= amount * size;
	}

	UpdateGhost();

	canManuallyDrop = true;
}

void Tetris1P::RotatePlayer( int dir )
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

	UpdateGhost();

	canManuallyDrop = true;
}

void Tetris1P::ResetPlayer()
{
	// piece = Tetreon::GetRandPiece();
	piece = nextPieces[0];
	ShiftNextPieces();
	// nextPieces[nNextPieces - 1] = Tetreon::GetRandPiece();
	nextPieces[nNextPieces - 1] = Tetreon::GetPiece( GetNextBagItem() );

	ResetPlayerPos();

	canManuallyDrop = true;
}

void Tetris1P::ResetPlayerPos()
{
	piece.GetPos().x = Arena::width / 2 * size;
	piece.GetPos().y = 0;

	dropTimer.Reset();

	// End game if true.
	if( arena.Collide( piece ) )
	{
		// arena.Clear();
		// ResetPlayer();
		gameOver = true;
	}

	UpdateGhost();
}

void Tetris1P::ShiftNextPieces()
{
	// for( int i = nNextPieces - 2; i >= 0; --i )
	// {
	// 	nextPieces[i] = nextPieces[i + 1];
	// }
	for( int i = 0; i < nNextPieces - 1; ++i )
	{
		nextPieces[i] = nextPieces[i + 1];
	}
}

void Tetris1P::SwapStored()
{
	if( storedPiece.GetType() != Tetreon::Type::Fake )
	{
		std::swap( piece,storedPiece );
		ResetPlayerPos(); // Doesn't change piece!
	}
	else
	{
		storedPiece = piece;
		ResetPlayer(); // Changes piece!
	}
}

void Tetris1P::RandomizeBag()
{
	// Wow this is gross but I always mess up for loops.
	bag[0] = Tetreon::Type::T;
	bag[1] = Tetreon::Type::O;
	bag[2] = Tetreon::Type::L;
	bag[3] = Tetreon::Type::J;
	bag[4] = Tetreon::Type::I;
	bag[5] = Tetreon::Type::S;
	bag[6] = Tetreon::Type::Z;
	// std::shuffle( bag.begin(),bag.end(),rngEng );

	// Using my own shuffle func cuz the std one is bad.
	const int nShuffles = 10;
	for( int i = 0; i < nShuffles; ++i )
	{
		std::swap( bag[Random::RangeI( 0,6 )],
			bag[Random::RangeI( 0,6 )] );
	}
}

Tetreon::Type Tetris1P::GetNextBagItem()
{
	auto temp = bag[curBagSpot];
	bag[curBagSpot] = Tetreon::Type::Fake;
	++curBagSpot;
	if( curBagSpot >= bagSize )
	{
		curBagSpot = 0;
		RandomizeBag();
	}
	return( temp );
}
