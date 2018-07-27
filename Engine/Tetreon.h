#pragma once

#include <vector>
#include <cassert>
#include "Random.h"

class Tetreon
{
public:
	enum class Type
	{
		Fake,T,O,L,J,I,S,Z
	};
private:
	typedef unsigned int uint;
public:
	Tetreon()
		:
		Tetreon( { 1,1,1,1,1,1,1,1,1 },
			{ 0,0 },Type::Fake )
	{}
	Tetreon( const std::vector<uint>& data,
		const Vei2& pos,Type t )
		:
		pos( pos ),
		dim( ( data.size() == 3 * 3 ) ? 3 : 4 ),
		myType( t )
	{
		assert( data.size() == 3 * 3 ||
			data.size() == 4 * 4 );
		matrix.reserve( dim * dim );
		for( int i = 0; i < dim * dim; ++i )
		{
			matrix.emplace_back( data[i] );
		}
	}
	// Tetreon( const std::vector<uint>& data )
	// 	:
	// 	Tetreon( data,Vei2( 0,0 ) )
	// {}
	const std::vector<uint>& GetMat() const
	{
		return( matrix );
	}
	const Vei2& GetPos() const
	{
		return( pos );
	}
	uint& GetDataAt( int x,int y )
	{
		return( matrix[y * dim + x] );
	}
	Vei2& GetPos()
	{
		return( pos );
	}
	static Tetreon GetRandPiece()
	{
		const int rnd = Random::RangeI( 0,6 );
		switch( rnd )
		{
		case 0: return( Tetreon::T() ); break;
		case 1: return( Tetreon::O() ); break;
		case 2: return( Tetreon::L() ); break;
		case 3: return( Tetreon::J() ); break;
		case 4: return( Tetreon::I() ); break;
		case 5: return( Tetreon::S() ); break;
		case 6: return( Tetreon::Z() ); break;
		default: // You will never get this.
			assert( false );
			return( Tetreon() );
			break;
		}
	}
	static Tetreon Rotate( const Tetreon& piece,int dir )
	{
		const auto& matVec = piece.GetMat();
		const int dimension = piece.GetDim();

		// TODO: Make this less ugly.
		if( matVec.size() == 3 * 3 )
		{
			uint mat[dimS][dimS] =
			{
				{ matVec[0],matVec[1],matVec[2] },
				{ matVec[3],matVec[4],matVec[5] },
				{ matVec[6],matVec[7],matVec[8] }
			};

			for( int y = 0; y < dimension; ++y )
			{
				for( int x = 0; x < y; ++x )
				{
					std::swap( mat[x][y],mat[y][x] );
					// std::swap( matVec[y * dimension + x],
					// 	matVec[x * dimension + x] );
				}
			}

			if( dir > 0 )
			{
				for( int i = 0; i < piece.GetDim(); ++i )
				{
					std::reverse( std::begin( mat[i] ),
						std::end( mat[i] ) );
				}
			}
			else
			{
				std::reverse( std::begin( mat ),
					std::end( mat ) );
			}

			std::vector<uint> newData;
			for( int i = 0; i < piece.GetDim(); ++i )
			{
				for( int j = 0; j < piece.GetDim(); ++j )
				{
					newData.emplace_back( mat[i][j] );
				}
			}

			const Tetreon temp{ newData,
				piece.GetPos(),piece.GetType() };
			return( temp );
		}
		else
		{
			uint mat[dimL][dimL] =
			{
				{ matVec[0],matVec[1],matVec[2],matVec[3] },
				{ matVec[4],matVec[5],matVec[6],matVec[7] },
				{ matVec[8],matVec[9],matVec[10],matVec[11] },
				{ matVec[12],matVec[13],matVec[14],matVec[15] },
			};

			for( int y = 0; y < dimension; ++y )
			{
				for( int x = 0; x < y; ++x )
				{
					std::swap( mat[x][y],mat[y][x] );
				}
			}

			if( dir > 0 )
			{
				for( int i = 0; i < piece.GetDim(); ++i )
				{
					std::reverse( std::begin( mat[i] ),
						std::end( mat[i] ) );
				}
			}
			else
			{
				std::reverse( std::begin( mat ),
					std::end( mat ) );
			}

			std::vector<uint> newData;
			for( int i = 0; i < piece.GetDim(); ++i )
			{
				for( int j = 0; j < piece.GetDim(); ++j )
				{
					newData.emplace_back( mat[i][j] );
				}
			}

			const Tetreon temp{ newData,
				piece.GetPos(),piece.GetType() };
			return( temp );
		}
	}
	static Tetreon Blank()
	{
		return( Tetreon( {
			0,0,0,
			0,0,0,
			0,0,0
			},Vei2( 0,0 ),Type::Fake ) );
	}
	static Tetreon T( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,1,0,
			1,1,1,
			0,0,0
			},pos,Type::T ) );
	}
	static Tetreon O( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			2,2,0,
			2,2,0,
			0,0,0
			},pos,Type::O ) );
	}
	static Tetreon L( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,3,0,
			0,3,0,
			0,3,3
			},pos,Type::L ) );
	}
	static Tetreon J( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,4,0,
			0,4,0,
			4,4,0
			},pos,Type::J ) );
	}
	static Tetreon I( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,5,0,0,
			0,5,0,0,
			0,5,0,0,
			0,5,0,0
			},pos,Type::I ) );
	}
	static Tetreon S( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,6,6,
			6,6,0,
			0,0,0
			},pos,Type::S ) );
	}
	static Tetreon Z( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			7,7,0,
			0,7,7,
			0,0,0
			},pos,Type::Z ) );
	}
	int GetDim() const
	{
		return( dim );
	}
	Type GetType() const
	{
		return( myType );
	}
public:
	static constexpr int dimS = 3;
	static constexpr int dimL = 4;
	static constexpr int size = 24;
	static constexpr Color tetCols[] =
	{
		Colors::MakeRGB( 255u,0u,255u ), // pls no
		Colors::MakeRGB( 221u,74u,192u ), // Purple,
		Colors::MakeRGB( 255u,182u,24u ), // Yellow,
		Colors::MakeRGB( 33u,89u,222u ), // Blue,
		Colors::MakeRGB( 252u,148u,46u ), // Orange,
		Colors::MakeRGB( 9u,174u,247u ), // Light Blue,
		Colors::MakeRGB( 99u,199u,16u ), // Green,
		Colors::MakeRGB( 247u,32u,57u ) // Salmon.
	};
private:
	/*const */Type myType;
	std::vector<uint> matrix;
	/*const */int dim;
	// uint matrix[][];
	Vei2 pos;
};