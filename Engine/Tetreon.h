#pragma once

#include <vector>
#include <cassert>

class Tetreon
{
private:
	typedef unsigned int uint;
public:
	Tetreon( const std::vector<uint>& data,
		const Vei2& pos )
		:
		pos( pos ),
		dim( ( data.size() == 3 * 3 ) ? 3 : 4 )
	{
		assert( data.size() == 3 * 3 ||
			data.size() == 4 * 4 );
		matrix.reserve( dim * dim );
		for( int i = 0; i < dim * dim; ++i )
		{
			matrix.emplace_back( data[i] );
		}
	}
	Tetreon( const std::vector<uint>& data )
		:
		Tetreon( data,Vei2( 0,0 ) )
	{}
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

			const Tetreon temp{ newData,piece.GetPos() };
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

			const Tetreon temp{ newData,piece.GetPos() };
			return( temp );
		}
	}
	static Tetreon T( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,0,0,
			1,1,1,
			0,1,0
			},pos ) );
	}
	static Tetreon O( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			1,1,0,
			1,1,0,
			0,0,0
			},pos ) );
	}
	static Tetreon L( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,1,0,
			0,1,0,
			0,1,1
			},pos ) );
	}
	static Tetreon J( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,1,0,
			0,1,0,
			1,1,0
			},pos ) );
	}
	static Tetreon I( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,1,0,0,
			0,1,0,0,
			0,1,0,0,
			0,1,0,0
			},pos ) );
	}
	static Tetreon S( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			0,1,1,
			1,1,0,
			0,0,0
			},pos ) );
	}
	static Tetreon Z( const Vei2& pos = Vei2( 0,0 ) )
	{
		return( Tetreon( {
			1,1,0,
			0,1,1,
			0,0,0
			},pos ) );
	}
	int GetDim() const
	{
		return( dim );
	}
public:
	static constexpr int dimS = 3;
	static constexpr int dimL = 4;
	static constexpr int size = 20;
private:
	std::vector<uint> matrix;
	/*const */int dim;
	// uint matrix[][];
	Vei2 pos;
};