#pragma once

#include "Vec2.h"
#include "Rect.h"
#include <string>
#include "Mouse.h"
#include "Graphics.h"
#include "Font.h"

class Button
{
public:
	Button( const Vei2& pos,const std::string& message,
		const Font& fixedSys )
		:
		pos( pos ),
		clickArea( CalculateClickArea( pos,
			message.length() ) ),
		msg( message ),
		fixedSys( fixedSys )
	{}
	void Update( const Mouse& mouse )
	{
		const auto& mousePos = mouse.GetPos();
		const auto mouseDown = mouse.LeftIsPressed();

		pressed = ( canClick && mouseDown );

		hovering = clickArea.ContainsPoint( mousePos );
		canClick = ( hovering && !mouseDown );
	}
	void Draw( Graphics& gfx ) const
	{
		const auto drawColor = ( hovering )
			? Colors::LightGray
			: Colors::Gray;

		gfx.DrawRect( pos.x,pos.y,
			clickArea.GetWidth(),
			clickArea.GetHeight(),
			drawColor );

		fixedSys.DrawText( msg,pos,Colors::White,gfx );
	}
	bool IsPressed() const
	{
		return( pressed );
	}
private:
	RectI CalculateClickArea( const Vei2& start,
		int strLen ) const
	{
		const auto width = 16 * strLen;
		const auto height = 28;

		return( RectI( start,width,height ) );
	}
private:
	const Vei2 pos;
	const RectI clickArea;
	const std::string msg;
	const Font& fixedSys;
	bool hovering = false;
	bool canClick = false;
	bool pressed = false;
};