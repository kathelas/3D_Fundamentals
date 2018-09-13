#pragma once

#include "Scene.h"
#include "Cube.h"
#include "PubeScreenTransformer.h"
#include "Mat3.h"

class TexCubeScene : public Scene
{
public:
	TexCubeScene() = default;
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
		auto triangles = cube.GetTrianglesTex();

		//generate rotation matrix
		const Mat3 rot =
			Mat3::RotateX( thetaX ) *
			Mat3::RotateY( thetaY ) *
			Mat3::RotateZ( thetaZ );

		//transform from model space into "view" (world) space
		for( auto& v : triangles.vertices )
		{
			v.pos *= rot;
			v.pos += { 0.0f, 0.0f, zOffset };
		}

		//culling backface triangles
		for( size_t i = 0, end = triangles.indices.size() / 3; i < end; i++ )
		{
			const Vec3& v0 = triangles.vertices[triangles.indices[i * 3]].pos;
			const Vec3& v1 = triangles.vertices[triangles.indices[i * 3 + 1]].pos;
			const Vec3& v2 = triangles.vertices[triangles.indices[i * 3 + 2]].pos;
			triangles.cullFlags[i] = ((v1 - v0).VectorProd( v2 - v0 )) * v0 > 0.0f;
		}

		//transform from view space into screen space, including perspective
		for( auto& v : triangles.vertices )
		{
			pst.Transform( v.pos );
		}

		//actual drawing
		for( size_t i = 0, end = triangles.indices.size() / 3; i < end; i++ )
		{
			if( !triangles.cullFlags[i] )
			{
				gfx.DrawTriangleTex(
					triangles.vertices[triangles.indices[i * 3]],
					triangles.vertices[triangles.indices[i * 3 + 1]],
					triangles.vertices[triangles.indices[i * 3 + 2]],
					texture );
			}
		}
	}

private:
	PubeScreenTransformer pst;
	Cube cube = Cube( 1.0f );

	float zOffset = 2.0f;
	static constexpr float dtheta = PI;
	float thetaX = 0.0f;
	float thetaY = 0.0f;
	float thetaZ = 0.0f;

	Surface texture = Surface::FromFile( L"pics\\grass.png" );
};