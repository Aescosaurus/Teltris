#pragma once

#include "Colors.h"
#include "Vec2.h"
#include <vector>
#include "Graphics.h"
#include <string>

class Poly
{
public:
	Poly( std::initializer_list<Vec2> vList );
	Poly( std::string filePath );

	void Draw( Graphics& gfx ) const;

	void Rotate( float amount );
private:
	Vec2 pos = Vec2{ 200.0f,200.0f };
	float angle = 0.0f;
	Color c = Colors::Magenta;
	const std::vector<Vec2> vertices;
};