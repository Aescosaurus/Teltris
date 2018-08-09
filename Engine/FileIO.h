#pragma once

#include <fstream>
#include <string>
#include <cassert>

class FileIO
{
public:
	static std::string Read( const std::string& fileName )
	{
		std::ifstream in{ fileName };
		assert( "FileIO doesn't exist!" && in.good() );

		std::string contents = "";

		for( char c = in.get(); in.good(); c = in.get() )
		{
			contents += c;
		}

		return( contents );
	}
	static void Create( const std::string& fileName )
	{
		{
			std::ifstream in{ fileName };
			assert( "FileIO already exists!" && !in.good() );
		}
		std::ofstream out{};
		out.open( fileName,std::ios::out );
	}
	static void Append( const std::string& fileName,
		const std::string& addition )
	{
		{
			std::ifstream in{ fileName };
			assert( "FileIO doesn't exist!" && in.good() );
		}

		std::ofstream out{ fileName };
		out << addition;
	}
	static bool Exists( const std::string& fileName )
	{
		std::ifstream in{ fileName };
		return( in.good() );
	}
private:
	FileIO()
	{}
};