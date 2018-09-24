#pragma once
#include "Vec2.h"

class SameSideTri
{
public:
	SameSideTri( Vec2& a, Vec2& b, Vec2& c )
		:
		a( a ),
		b( b ),
		c( c )
	{

	}
	SameSideTri( const Vec2& a, const Vec2& b, const Vec2& c )
		:
		a( a ),
		b( b ),
		c( c )
	{

	}

	Vec2 a;
	Vec2 b;
	Vec2 c;
};