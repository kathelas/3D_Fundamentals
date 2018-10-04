#pragma once
#include "Vec3.h"
#include "Graphics.h"

class PubeScreenTransformer
{
public:
	PubeScreenTransformer()
		:
		xFactor( float( Graphics::ScreenWidth ) / 2.0f ),
		yFactor( float( Graphics::ScreenHeight ) / 2.0f )
	{}
	template<class Vertex>
	Vertex& Transform( Vertex& v ) const
	{
		//perspective divide
		//divide all components of the vertex by z, so that we interpolate all components in the same space
		const float zInv = 1.0f / v.pos.z;

		v *= zInv;
		
		//adjusting x and y
		v.pos.x = ( v.pos.x + 1.0f ) * xFactor;
		v.pos.y = ( -v.pos.y + 1.0f ) * yFactor;

		//storing 1/z in z for later use to recover our vertex attributes after interpolating
		v.pos.z = zInv;

		return v;
	}
	template<class Vertex>
	Vertex GetTransformed( const Vertex& v ) const
	{
		return Transform( Vertex( v ) );
	}
private:
	float xFactor;
	float yFactor;
};