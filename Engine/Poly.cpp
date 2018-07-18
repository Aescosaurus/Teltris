#include "Poly.h"

Poly::Poly( std::initializer_list<Vec2> vList )
	:
	vertices( vList )
{
}

void Poly::Draw( Graphics& gfx ) const
{
	for( auto i = vertices.begin(),end = vertices.end() - 1; i != end; ++i )
	{
		const Vec2 pos1 = i->Rotation( angle ) + pos;
		const Vec2 pos2 = ( i + 1 )->Rotation( angle ) + pos;
		gfx.DrawLineOld( int( pos1.x ),int( pos1.y ),int( pos2.x ),int( pos2.y ),c );
	}
	const Vec2 pos1 = vertices.back().Rotation( angle ) + pos;
	const Vec2 pos2 = vertices.front().Rotation( angle ) + pos;
	gfx.DrawLineOld( int( pos1.x ),int( pos1.y ),int( pos2.x ),int( pos2.y ),c );
}

void Poly::Rotate( float amount )
{
	angle += amount;
}
