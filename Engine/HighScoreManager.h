#pragma once

#include "FileIO.h"
#include <string>
#include <cassert>
#include <vector>

class HighScoreManager
{
public:
	HighScoreManager()
	{
		const std::string file = scoreFilePath;
		if( !FileIO::Exists( file ) )
		{
			FileIO::Create( file );
			// for( int i = 0; i < nMaxScores; ++i )
			// {
			// 	FileIO::Append( file,"NONAM: 0\n" );
			// }
			std::vector<int> scores;
			std::vector<std::string> names;
			for( int i = 0; i < nMaxScores; ++i )
			{
				scores.emplace_back( 0 );
				names.emplace_back( "NONAM" );
			}
			RewriteFile( scores,names );
		}
	}
	void AddScore( int newScore,std::string name )
	{
		// Name needs to be 5 characters!!!
		while( name.length() < 5 ) name += '_';
		assert( name.length() == 5 );

		auto scores = GetScores();
		auto names = GetNames();

		assert( names.size() == scores.size() );

		for( int i = 0; i < int( scores.size() ); ++i )
		{
			if( newScore > scores[i] )
			{
				// Shift everything down.
				for( int j = int( scores.size() - 1 );
					j > i; --j )
				{
					if( j - 1 < 0 ) continue;

					scores[j] = scores[j - 1];
					names[j] = names[j - 1];
				}

				scores[i] = newScore;
				names[i] = name;

				break;
			}
		}

		RewriteFile( scores,names );
	}
	// These are highest to lowest I think.
	std::vector<std::string> GetNames() const
	{
		auto data = FileIO::Read( scoreFilePath );

		std::vector<std::string> names;

		for( int i = 0; i < int( data.length() ); ++i )
		{
			std::string tempName = "";
			bool gotValidName = false;

			if( i + 5 <= int( data.length() ) &&
				data[i + 5] == ':' &&
				data[i + 5 + 1] == ' ' )
			{
				bool finishedName = false;
				int j = 0;
				while( !finishedName )
				{
					const auto info = data[i + 0 + j];

					if( info == ':' ) finishedName = true;
					else tempName += info;

					++j;
				}

				gotValidName = true;
			}

			if( gotValidName )
			{
				names.emplace_back( tempName );
			}
		}
		
		return( names );
	}
	std::vector<int> GetScores() const
	{
		auto data = FileIO::Read( scoreFilePath );

		std::vector<int> nums;

		for( int i = 0; i < int( data.length() ); ++i )
		{
			std::string tempNum = "";
			bool gotValidNum = false;

			if( data[i] == ':' &&
				data[i + 1] == ' ' )
			{
				bool finishedNum = false;
				int j = 0;
				while( !finishedNum )
				{
					const auto info = data[i + 2 + j];
					if( info == '\n' ) finishedNum = true;
					else tempNum += info;

					++j;
				}

				gotValidNum = true;
			}

			if( gotValidNum )
			{
				nums.emplace_back( std::stoi( tempNum ) );
			}
		}

		return( nums );
	}
private:
	void RewriteFile( const std::vector<int>& nums,
		const std::vector<std::string>& names )
	{
		assert( nums.size() == names.size() );

		FileIO::Empty( scoreFilePath );
		
		for( int i = 0; i < int( nums.size() ); ++i )
		{
			std::string text = "";
			text += names[i];
			text += ": ";
			text += std::to_string( nums[i] );
			text += "\n";
			FileIO::Append( scoreFilePath,text );
		}
	}
private:
	static constexpr int nMaxScores = 9;
	const std::string scoreFilePath = "_HighScores.txt";
};