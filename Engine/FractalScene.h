#pragma once
#include  "Scene.h"
#include "Transformer.h"
#include "SameSideTri.h"


class FractalScene : public Scene
{
public:
	FractalScene( Graphics& gfx )
		:
		gfx( gfx ),
		tri( Vec2{ -0.5f, 0.0f }, Vec2{ 0.5f, 0.0f }, Vec2{ 0.0f, sqrt( 3.0f ) / 2 } )
	{



	}
	virtual void Update( Keyboard& kbd, Mouse& mouse, float dt ) override
	{

		if( kbd.KeyIsPressed( 'Q' ) )
		{
			offsetz = offsetz + dt * offsetz * offsetmulti;
		}
		if( kbd.KeyIsPressed( 'A' ) )
		{
			offsetz = offsetz - dt * offsetz * offsetmulti;;
		}
		if( kbd.KeyIsPressed( VK_LEFT ) )
		{
			Xmove( tri, -(dt * scrollspeed * offsetz) );
		}
		if( kbd.KeyIsPressed( VK_RIGHT ) )
		{
			Xmove( tri, (dt * scrollspeed * offsetz) );
		}
		if( kbd.KeyIsPressed( VK_UP ) )
		{
			Ymove( tri, -(dt * scrollspeed * offsetz) );
		}
		if( kbd.KeyIsPressed( VK_DOWN ) )
		{
			Ymove( tri, (dt * scrollspeed * offsetz) );
		}


	}
	virtual void Draw() override
	{
		SameSideTri temptri = tri;
		trans.Transform( temptri.a, offsetz );
		trans.Transform( temptri.b, offsetz );
		trans.Transform( temptri.c, offsetz );
		DrawTriangle( temptri, Colors::White );
		DrawTriangleInside( tri );

	}
private:
	void DrawTriangle( const SameSideTri& triangle, Color c )
	{
		//pointers for swapping to sort
		const Vec2* pv0 = &triangle.a;
		const Vec2* pv1 = &triangle.b;
		const Vec2* pv2 = &triangle.c;

		//sorting by y
		if( pv1->y < pv0->y ) std::swap( pv0, pv1 );
		if( pv2->y < pv1->y ) std::swap( pv1, pv2 );
		if( pv1->y < pv0->y ) std::swap( pv0, pv1 );

		//flattop
		if( pv0->y == pv1->y )
		{
			//sorting top by x
			if( pv1->x < pv0->x ) std::swap( pv0, pv1 );

			DrawFlatTopTriangle( *pv0, *pv1, *pv2, c );
		}

		//flatbot
		else if( pv1->y == pv2->y )
		{
			//sorting bot by x
			if( pv2->x < pv1->x ) std::swap( pv1, pv2 );

			DrawFlatBotTriangle( *pv0, *pv1, *pv2, c );
		}

	}
	void DrawFlatBotTriangle( const Vec2& it0, const Vec2& it1, const Vec2& it2, Color c )
	{

		//calc dVertex / dy, change in interpolant for every 1 change in y
		const float deltay = it2.y - it0.y;
		const auto dit0 = (it1 - it0) / deltay;
		const auto dit1 = (it2 - it0) / deltay;

		//create right edge interpolant
		Vec2 itedge1 = it0;

		DrawFlatTriangle( it0, it1, it2, dit0, dit1, itedge1, c );
	}
	void DrawFlatTopTriangle( const Vec2& it0, const Vec2& it1, const Vec2& it2, Color c )
	{

		//calc dVertex / dy, change in interpolant for every 1 change in y
		const float deltay = it2.y - it0.y;
		const auto dit0 = (it2 - it0) / deltay;
		const auto dit1 = (it2 - it1) / deltay;

		//create right edge interpolant
		Vec2 itedge1 = it1;

		DrawFlatTriangle( it0, it1, it2, dit0, dit1, itedge1, c );
	}

	void DrawFlatTriangle( const Vec2& it0, const Vec2& it1, const Vec2& it2, const Vec2& dv0, const Vec2& dv1, Vec2 itedge1, Color c )
	{
		//create left egde interpolant, always it0
		auto itedge0 = it0;

		//calc y start and end
		const int yStart = (int)ceil( it0.y - 0.5f );
		const int yEnd = (int)ceil( it2.y - 0.5f ); //line after the last line drawn

		//interpolant prestep (because you start drawing the triangle at the middle of the first pixel not right at the vertex)
		itedge0 += dv0 * (float( yStart ) + 0.5f - it0.y);
		itedge1 += dv1 * (float( yStart ) + 0.5f - it0.y);

		for( int y = yStart; y < yEnd; y++, itedge0 += dv0, itedge1 += dv1 )
		{
			//calc start and end pixels
			const int xStart = (int)ceil( itedge0.x - 0.5f );
			const int xEnd = (int)ceil( itedge1.x - 0.5f );

			//create line interpolant start
			//not needed to interpolate x,y,z, but makes things easier and will be needed in the future
			auto iline = itedge0;

			//calc delta line interpolant / dx
			const float dx = itedge1.x - itedge0.x;
			const auto diline = (itedge1 - iline) / dx;

			//prestep line interpolant
			iline += diline * (float( xStart ) + 0.5f - itedge0.x);

			for( int x = xStart; x < xEnd; x++, iline += diline )
			{
				//invoking pixelshader
				if( x > 0 && x < Graphics::ScreenWidth && y > 0 && y < Graphics::ScreenHeight )
					gfx.PutPixel( x, y, c );
			}
		}
	}
	void DrawTriangleInside( const SameSideTri& tri )
	{


		Vec2 newa = tri.a + (tri.c - tri.a) / 2.0f;
		Vec2 newb = tri.a + (tri.b - tri.a) / 2.0f;
		Vec2 newc = tri.b + (tri.c - tri.b) / 2.0f;

		//pointers for swapping to sort
		const Vec2* pa = &newa;
		const Vec2* pb = &newb;
		const Vec2* pc = &newc;



		SameSideTri newtri = { *pa, *pb, *pc };

		SameSideTri tri1 = { tri.a, newa, newb };
		SameSideTri tri2 = { newa, tri.c, newc };
		SameSideTri tri3 = { newb, tri.b, newc };

		trans.Transform( newtri.a, offsetz );
		trans.Transform( newtri.b, offsetz );
		trans.Transform( newtri.c, offsetz );

		if( (newtri.a - newtri.b).LenSq() < 5.0f )
		{
			return;
		}

		DrawTriangleInside( tri1 );
		DrawTriangleInside( tri2 );
		DrawTriangleInside( tri3 );


		if( (newtri.a.x < 0 || newtri.a.y < 0 || newtri.a.x > Graphics::ScreenWidth || newtri.a.y > Graphics::ScreenHeight) &&
			(newtri.b.x < 0 || newtri.b.y < 0 || newtri.b.x > Graphics::ScreenWidth || newtri.b.y > Graphics::ScreenHeight) &&
			(newtri.c.x < 0 || newtri.c.y < 0 || newtri.c.x > Graphics::ScreenWidth || newtri.c.y > Graphics::ScreenHeight) )
		{
			//pointers for swapping to sort
			const Vec2* pv0 = &newtri.a;
			const Vec2* pv1 = &newtri.b;
			const Vec2* pv2 = &newtri.c;

			//sorting by y
			if( pv1->y < pv0->y ) std::swap( pv0, pv1 );
			if( pv2->y < pv1->y ) std::swap( pv1, pv2 );
			if( pv1->y < pv0->y ) std::swap( pv0, pv1 );

			if( pv0->y == pv1->y )
			{
				if( pv1->x < pv0->x ) std::swap( pv0, pv1 );
			}




			if( pv0->y > Graphics::ScreenHeight ||
				pv2->y < 0.0f ||
				(pv2->y < pv2->x * negslope) ||
				(pv2->y < ((pv2->x - (float)Graphics::ScreenWidth) * slope)) ||
				pv0->x > Graphics::ScreenWidth ||
				pv1->x < 0.0f
				)
			{
				return;
			}
			else
			{
				DrawTriangle( newtri, Colors::Gray );
			}
		}
		else
		{
			DrawTriangle( newtri, Colors::Gray );
		}

	}

	SameSideTri& Xmove( SameSideTri& tri, float x )
	{
		tri.a.x += x;
		tri.b.x += x;
		tri.c.x += x;
		return tri;
	}

	SameSideTri& Ymove( SameSideTri& tri, float y )
	{
		tri.a.y += y;
		tri.b.y += y;
		tri.c.y += y;
		return tri;
	}

private:
	Graphics& gfx;
	float offsetz = 2;
	SameSideTri tri;
	Transformer trans;
	float scrollspeed = 1.0f;
	float offsetmulti = 0.20f;
	float slope = sqrt( 3 );
	float negslope = sqrt( 3 ) * float(-1);
};