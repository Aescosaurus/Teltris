#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <string>

class Button
{
public:
	Button( const Vei2& pos,const Vei2& size,
		const std::string& message )
		:
		pos( pos ),
		clickArea( pos,size.x,size.y ),
		msg( message )
	{}
	void Update( const Vei2& mousePos )
	{
		
	}
private:
	const Vei2 pos;
	const RectI clickArea;
	const std::string msg;
};