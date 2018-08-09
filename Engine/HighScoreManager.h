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
			for( int i = 0; i < nMaxScores; ++i )
			{
				FileIO::Append( file,"NONAM: 0\n" );
			}
		}
	}
	void UpdateScoreboard( int newScore,
		std::string name )
	{
		while( name.length < 5 ) name += '_';
		for( char& c : name )
		{
			if( c == ' ' ) c = '_';
		}
		assert( name.length == 5 );

		const auto numAsStr = std::to_string( newScore );

		std::string rawData = FileIO::Read( scoreFilePath );
		const auto scores = GetScores();
		for( int i = 0; i < int( scores.size() ); ++i )
		{
			if( newScore > scores[i] )
			{
				// Should be i - 1, but i starts at 0.
				const int targetLine = i;
				// Because 1st line is 1, not 0;
				int curLine = 1;

				for( int j = 0; j < rawData.length(); ++j )
				{
					if( rawData[j] == '\n' )
					{
						++curLine;

						// Replace name and score.
						if( curLine == targetLine )
						{
							bool atEndOfLine = false;
							// k starts at 1 to skip prev newline.
							int k = 1;
							while( !atEndOfLine )
							{
								assert( k <= 5 + 1 + 1 );
								if( rawData[j + k] == ':' )
								{
									atEndOfLine = true;
								}

								rawData[j + k] = name[k - 1];

								++k;
							}
							bool keepGoing = false;
							int l = 0;
							while( keepGoing )
							{
								rawData[j + k + l] = numAsStr[l];

								if( rawData[j + k + l] == '\n' )
								{
									for( int m = l;
										m < numAsStr.length();
										++m )
									{
										rawData.insert( rawData.begin() + j + k + l,numAsStr[m] );
									}
									keepGoing = false;
								}

								++l;
							}
						}
					}
				}
			}
		}
	}
	std::vector<int> GetScores()
	{
		auto data = FileIO::Read( scoreFilePath );

		std::vector<int> nums;

		for( int i = 0; i < data.length(); ++i )
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
	static constexpr int nMaxScores = 10;
	const std::string scoreFilePath = "HighScores.scr";
};