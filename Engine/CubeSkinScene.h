#pragma once

#include "Scene.h"
#include "Cube.h"
#include "Mat3.h"
#include "Pipeline.h"
#include "TextureEffect.h"

class CubeSkinScene : public Scene
{
public:
	typedef Pipeline<TextureEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	CubeSkinScene( Graphics& gfx, const std::wstring filename )
		:
		itlist( Cube::GetSkinned<Vertex>() ),
		pipeline( gfx )
	{
		pipeline.effect.ps.BindTexture( filename );
	}
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
	virtual void Draw() override
	{
		//rotation
		const Mat3 rot =
			Mat3::RotateX( thetaX ) *
			Mat3::RotateY( thetaY ) *
			Mat3::RotateZ( thetaZ );

		//translation
		const Vec3 trans = { 0.0f, 0.0f, zOffset };

		pipeline.BindRotation( rot );
		pipeline.BindTranslation( trans );

		//render triangles
		pipeline.Draw( itlist );
	}

private:
	IndexedTriangleList<Vertex> itlist;
	Pipeline pipeline;
	static constexpr float dtheta = PI;
	float zOffset = 2.0f;
	float thetaX = 0.0f;
	float thetaY = 0.0f;
	float thetaZ = 0.0f;

};