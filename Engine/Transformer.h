#pragma once
#include "Vec2.h"
#include "Graphics.h"

class Transformer
{
public:
	Vec2& Transform( Vec2& v )
	{
		v.x = (v.x + 1.0f) * xFactor;
		v.y = (-v.y + 1.0f) * yFactor;
		return v;
	}
private:
	float xFactor = float( Graphics::ScreenWidth ) / 2.0f;
	float yFactor = float( Graphics::ScreenHeight ) / 2.0f;
};