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
	TexVertex Interpolate( const TexVertex& dest, float alpha ) const
	{
		return {
			pos.Interpolate( dest.pos, alpha ),
			postex.Interpolate( dest.postex, alpha )
		};
	}
	TexVertex& operator+=( const TexVertex& rhs )
	{
		pos += rhs.pos;
		postex += rhs.postex;
		return *this;
	}
	TexVertex operator+( const TexVertex& rhs ) const
	{
		return TexVertex( *this ) += rhs;
	}
	TexVertex& operator-=( const TexVertex& rhs )
	{
		pos -= rhs.pos;
		postex -= rhs.postex;
		return *this;
	}
	TexVertex operator-( const TexVertex& rhs ) const
	{
		return TexVertex( *this ) -= rhs;
	}
	TexVertex& operator*=( float rhs )
	{
		pos *= rhs;
		postex *= rhs;
		return *this;
	}
	TexVertex operator*( float rhs ) const
	{
		return TexVertex( *this ) *= rhs;
	}
	TexVertex& operator/=( float rhs )
	{
		pos /= rhs;
		postex /= rhs;
		return *this;
	}
	TexVertex operator/( float rhs ) const
	{
		return TexVertex( *this ) /= rhs;
	}

public:
	Vec3 pos;		//position on the model
	Vec2 postex;		//coordinates in the texture
};