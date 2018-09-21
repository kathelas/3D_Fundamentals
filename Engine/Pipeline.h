#pragma once
#include "ChiliWin.h"
#include "Graphics.h"
#include "PubeScreenTransformer.h"
#include "Mat3.h"
#include "Triangle.h"
#include "IndexedTriangleList.h"

template<class Effect>
class Pipeline
{
	//pipeline with configurable pixel shader effect
	//draws textured triangle lists
public:
	typedef typename Effect::Vertex Vertex;

public:
	Pipeline( Graphics& gfx )
		:
		gfx( gfx )
	{}
	void Draw( IndexedTriangleList<Vertex>& trilist )
	{
		ProcessVertices( trilist.vertices, trilist.indices );
	}
	void BindRotation( const Mat3& rot_in )
	{
		rotation = rot_in;
	}
	void BindTranslation( const Vec3& transl_in )
	{
		translation = transl_in;
	}
private:
	//vertex processing
	//transform and rotate, then pass to triangle assembler
	void ProcessVertices( const std::vector<Vertex>& vertices, const std::vector<size_t>& indices )
	{
		std::vector<Vertex> verticesOut;

		for( const auto& v : vertices )
		{
			verticesOut.emplace_back( v.pos * rotation + translation, v.t );
		}

		AssembleTriangles( verticesOut, indices );
	}
	//triangle assembly
	//culls back facing triangles, then pass to triangle processing
	void AssembleTriangles( const std::vector<Vertex>& vertices, const std::vector<size_t>& indices )
	{
		for( size_t i = 0, end = indices.size() / 3; i < end; i++ )
		{
			const auto& v0 = vertices[indices[i * 3]];
			const auto& v1 = vertices[indices[i * 3 + 1]];
			const auto& v2 = vertices[indices[i * 3 + 2]];

			//culling (cross product)
			if( (v1.pos - v0.pos).VectorProd( v2.pos - v0.pos ) * v0.pos <= 0.0f )
			{
				ProcessTriangle( v0, v1, v2 );
			}
		}
	}
	//triangle processing
	//sends triangles to post processing
	void ProcessTriangle( const Vertex& v0, const Vertex& v1, const Vertex& v2 )
	{
		PostProcessTriangleVertices( Triangle<Vertex>{ v0, v1, v2 } );
	}
	//vertex post processing
	//perspective and viewport transformation
	void PostProcessTriangleVertices( Triangle<Vertex>& triangle )
	{
		//perspective divide + screen transform
		pst.Transform( triangle.v0.pos );
		pst.Transform( triangle.v1.pos );
		pst.Transform( triangle.v2.pos );

		DrawTriangle( triangle );
	}
	//triangle rasterization
	//it = interpolants, values which will be interpolated across the triangle in screen space; top left rule
	//first sort vertices, determine triangle type, split to flat triangles, pass to flat triangle functions
	void DrawTriangle( const Triangle<Vertex>& triangle )
	{
		//pointers for swapping to sort
		const Vertex* pv0 = &triangle.v0;
		const Vertex* pv1 = &triangle.v1;
		const Vertex* pv2 = &triangle.v2;

		//sorting by y
		if( pv1->pos.y < pv0->pos.y ) std::swap( pv0, pv1 );
		if( pv2->pos.y < pv1->pos.y ) std::swap( pv1, pv2 );
		if( pv1->pos.y < pv0->pos.y ) std::swap( pv0, pv1 );

		//flattop
		if( pv0->pos.y == pv1->pos.y )
		{
			//sorting top by x
			if( pv1->pos.x < pv0->pos.x ) std::swap( pv0, pv1 );

			DrawFlatTopTriangle( *pv0, *pv1, *pv2 );
		}

		//flatbot
		else if( pv1->pos.y == pv2->pos.y )
		{
			//sorting bot by x
			if( pv2->pos.x < pv1->pos.x ) std::swap( pv1, pv2 );

			DrawFlatBotTriangle( *pv0, *pv1, *pv2 );
		}

		//general triangle
		else
		{
			//find split
			const float alpha = (pv1->pos.y - pv0->pos.y) / (pv2->pos.y - pv0->pos.y);
			const auto vsplit = interpolate( *pv0, *pv2, alpha );

			//longest side on the right
			if( pv1->pos.x < vsplit.pos.x )
			{
				DrawFlatBotTriangle( *pv0, *pv1, vsplit );
				DrawFlatTopTriangle( *pv1, vsplit, *pv2 );
			}
			//longest side on the left
			else
			{
				DrawFlatBotTriangle( *pv0, vsplit, *pv1 );
				DrawFlatTopTriangle( vsplit, *pv1, *pv2);
			}
		}
	}
	//flat -TOP- specific calculations, then passes to flat triangle drawing
	void DrawFlatTopTriangle( const Vertex& it0, const Vertex& it1, const Vertex& it2 )
	{

		//calc dVertex / dy, change in interpolant for every 1 change in y
		const float deltay = it2.pos.y - it0.pos.y;
		const auto dit0 = (it2 - it0) / deltay;
		const auto dit1 = (it2 - it1) / deltay;

		//create right edge interpolant
		Vertex itedge1 = it1;

		DrawFlatTriangle( it0, it1, it2, dit0, dit1, itedge1 );
	}
	//flat -BOT- specific calculations, then passes to flat triangle drawing
	void DrawFlatBotTriangle( const Vertex& it0, const Vertex& it1, const Vertex& it2 )
	{

		//calc dVertex / dy, change in interpolant for every 1 change in y
		const float deltay = it2.pos.y - it0.pos.y;
		const auto dit0 = (it1 - it0) / deltay;
		const auto dit1 = (it2 - it0) / deltay;

		//create right edge interpolant
		Vertex itedge1 = it0;

		DrawFlatTriangle( it0, it1, it2, dit0, dit1, itedge1 );
	}
	//processing common to both flat triangles + texture lookup, clamping and pixel drawing
	void DrawFlatTriangle( const Vertex& it0, const Vertex& it1, const Vertex& it2, const Vertex& dv0, const Vertex& dv1, Vertex itedge1 )
	{
		//create left egde interpolant, always it0
		auto itedge0 = it0;

		//calc y start and end
		const int yStart =	(int)ceil( it0.pos.y - 0.5f );
		const int yEnd =		(int)ceil( it2.pos.y - 0.5f ); //line after the last line drawn

		//interpolant prestep (because you start drawing the triangle at the middle of the first pixel not right at the vertex)
		itedge0 += dv0 * (float( yStart ) + 0.5f - it0.pos.y);
		itedge1 += dv1 * (float( yStart ) + 0.5f - it0.pos.y);

		for( int y = yStart; y < yEnd; y++, itedge0 += dv0, itedge1 += dv1 )
		{
			//calc start and end pixels
			const int xStart =	(int)ceil( itedge0.pos.x - 0.5f );
			const int xEnd =		(int)ceil( itedge1.pos.x - 0.5f );

			//create line interpolant start
			//not needed to interpolate x,y,z, but makes things easier and will be needed in the future
			auto iline = itedge0;

			//calc delta line interpolant / dx
			const float dx = itedge1.pos.x - itedge0.pos.x;
			const auto diline = (itedge1 - iline) / dx;

			//prestep line interpolant
			iline += diline * (float( xStart ) + 0.5f - itedge0.pos.x);

			for( int x = xStart; x < xEnd; x++, iline += diline )
			{
				//invoking pixelshader
				gfx.PutPixel( x, y, effect.ps( iline ) );
			}
		}
	}

public:
	Effect effect;

private:
	Graphics& gfx;
	PubeScreenTransformer pst;
	Mat3 rotation;
	Vec3 translation;
};