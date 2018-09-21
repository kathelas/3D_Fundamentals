#pragma once

#include "Pipeline.h"

class TextureEffect
{
	//vertex type for this effect
public:

	class Vertex
	{
	public:
		Vertex() = default;
		Vertex( const Vec3& pos, const Vec2& t )
			:
			pos( pos ),
			t( t )
		{}
		Vertex( const Vec3& pos, Vertex& src )
			:
			pos( pos ),
			t( src.t )
		{}
		Vertex& operator+=( const Vertex& rhs )
		{
			pos += rhs.pos;
			t += rhs.t;
			return *this;
		}
		Vertex operator+( const Vertex& rhs ) const
		{
			return Vertex( *this ) += rhs;
		}
		Vertex& operator-=( const Vertex& rhs )
		{
			pos -= rhs.pos;
			t -= rhs.t;
			return *this;
		}
		Vertex operator-( const Vertex& rhs ) const
		{
			return Vertex( *this ) -= rhs;
		}
		Vertex& operator*=( float rhs )
		{
			pos *= rhs;
			t *= rhs;
			return *this;
		}
		Vertex operator*( float rhs ) const
		{
			return Vertex( *this ) *= rhs;
		}
		Vertex& operator/=( float rhs )
		{
			pos /= rhs;
			t /= rhs;
			return *this;
		}
		Vertex operator/( float rhs ) const
		{
			return Vertex( *this ) /= rhs;
		}

	public:
		Vec3 pos;
		Vec2 t;		//texture coordinates
	};

	class PixelShader
	{
	public:
		template<class Input>
		Color operator()( const Input& in ) const
		{
			return ptex->GetPixel(
				(unsigned int)std::min( in.t.x * texw + 0.5f, texclampx ),
				(unsigned int)std::min( in.t.y * texh + 0.5f, texclampy )
			);
			//making sure not to read of the texture edge (fp error)
		}
		
		void BindTexture( const std::wstring& filename )
		{
			ptex = std::make_unique<Surface>( Surface::FromFile( filename ) );
			texw = float( ptex->GetWidth() );
			texh = float( ptex->GetHeight() );
			texclampx = texw - 1.0f;
			texclampy = texh - 1.0f;
		}

	private:
		std::unique_ptr<Surface> ptex;
		float texw;
		float texh;
		float texclampx;
		float texclampy;
	};

public:
	PixelShader ps;
};