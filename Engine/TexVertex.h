#pragma once

#include "Vec2.h"
#include "Vec3.h"

class TexVertex
{
public:
	TexVertex( const Vec3& in_pos, const Vec2& in_postex )
		:
		pos( in_pos ),
		postex( in_postex )
	{}


	Vec3 pos;		//position on the model
	Vec2 postex;		//coordinates in the texture
};