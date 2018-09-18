#pragma once

#include "Scene.h"
#include "Cube.h"
#include "PubeScreenTransformer.h"
#include "Mat3.h"

class SolidCubeScene : public Scene
{
public:
	SolidCubeScene() = default;
	virtual void Update( Keyboard& kbd, Mouse& mouse, float dt ) override
	{
		if( kbd.KeyIsPressed( 'Q' ) )
		{
			thetaX = wrapangle( thetaX + dt * dtheta );
		}
		if( kbd.KeyIsPressed( 'A' ) )
		{
			thetaX = wrapangle( thetaX - dt * dtheta );
		}
		if( kbd.KeyIsPressed( 'W' ) )
		{
			thetaY = wrapangle( thetaY + dt * dtheta );
		}
		if( kbd.KeyIsPressed( 'S' ) )
		{
			thetaY = wrapangle( thetaY - dt * dtheta );
		}
		if( kbd.KeyIsPressed( 'E' ) )
		{
			thetaZ = wrapangle( thetaZ + dt * dtheta );
		}
		if( kbd.KeyIsPressed( 'D' ) )
		{
			thetaZ = wrapangle( thetaZ - dt * dtheta );
		}
		if( kbd.KeyIsPressed( 'R' ) )
		{
			zOffset = zOffset + dt;
		}
		if( kbd.KeyIsPressed( 'F' ) )
		{
			zOffset = zOffset - dt;
		}
	}
	virtual void Draw( Graphics& gfx ) override
	{
		auto triangles = cube.GetTriangles();

		//rotation
		const Mat3 rot =
			Mat3::RotateX( thetaX ) *
			Mat3::RotateY( thetaY ) *
			Mat3::RotateZ( thetaZ );

		//transform from model space into "view" (world) space
		for( auto& v : triangles.vertices )
		{
			v *= rot;
			v += { 0.0f, 0.0f, zOffset };
		}

		//culling backface triangles
		for( size_t i = 0, end = triangles.indices.size() / 3; i < end; i++ )
		{
			const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]];
			const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]];
			const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]];
			triangles.cullFlags[i] = ( ((v1 - v0).VectorProd( v2 - v0 )) * v0 >= 0.0f );
		}

		//transform from view space into screen space, including perspective
		for( auto& v : triangles.vertices )
		{
			pst.Transform( v );
		}

		//actual drawing
		for( size_t i = 0, end = triangles.indices.size() / 3; i < end; i++ )
		{
			if( !triangles.cullFlags[i] )
			{
				gfx.DrawTriangle(
					triangles.vertices[triangles.indices[i * 3]],
					triangles.vertices[triangles.indices[i * 3 + 1]],
					triangles.vertices[triangles.indices[i * 3 + 2]],
					colors[i] );
			}
		}
	}

private:
	PubeScreenTransformer pst;
	Cube cube = Cube( 1.0f );

	float zOffset = 2.0f;
	float dtheta = PI;
	float thetaX = 0.0f;
	float thetaY = 0.0f;
	float thetaZ = 0.0f;

	const Color colors[12] = {
		Colors::Blue,
		Colors::Green,
		Colors::White,
		Colors::Red,
		Colors::Yellow,
		Colors::Green,

		Colors::Magenta,
		Colors::Cyan,
		Colors::Gray,
		Colors::LightGray,
		Colors::Blue,
		Colors::MakeRGB( 50, 50, 50 ) };

};