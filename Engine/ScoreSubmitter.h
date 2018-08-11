#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Button.h"

class ScoreSubmitter
{
public:
	void Update( Keyboard& kbd,const Mouse& ms )
	{
		if( done ) return;
		if( !kbd.CharIsEmpty() )
		{
			auto data = kbd.ReadChar();
			if( data == '\b' )
			{
				if( name.length() > 0 )
				{
					name.erase( name.end() - 1 );
				}
			}
			else if( data == '\r' )
			{
				done = true;
				// Make name proper since we won't change.
				for( int i = 0; i < int( name.size() ); ++i )
				{
					if( name[i] == ' ' )
					{
						name.erase( name.begin() + i );
					}
				}
				while( name.length() > 5 )
				{
					name.erase( name.end() - 1 );
				}
				while( name.length() < 5 ) name += '_';
				assert( name.length() == 5 );
			}
			else if( data != 32 )
			{
				name += data;
			}
		}
	}
	void Draw( Graphics& gfx ) const
	{
		fixedSys.DrawText( "Name: " + name,{ 370,150 },
			Colors::White,gfx );
	}
	const std::string& GetName() const
	{
		return( name );
	}
	bool IsDone() const
	{
		return( done );
	}
private:
	/*const */Font fixedSys = "Fonts/FixedSys16x28.bmp";
	std::string name = "";
	bool done = false;
};