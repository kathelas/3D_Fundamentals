#pragma once

#include "Scene.h"
#include "Pipeline.h"
#include "VertexColorEffect.h"



class CubeColorEffectScene : public Scene
{
public:
	typedef Pipeline<VertexColorEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	CubeColorEffectScene( Graphics& gfx )
		:
		pipeline( gfx ),
		itlist( Cube::GetPlain<Vertex>() )
	{
		itlist.vertices[0].color = Vec3( Colors::Red );
		itlist.vertices[1].color = Vec3( Colors::Green );
		itlist.vertices[2].color = Vec3( Colors::Blue );
		itlist.vertices[3].color = Vec3( Colors::Yellow );
		itlist.vertices[4].color = Vec3( Colors::Cyan );
		itlist.vertices[5].color = Vec3( Colors::Magenta );
		itlist.vertices[6].color = Vec3( Colors::White );
		itlist.vertices[7].color = Vec3( Colors::Black );
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
		pipeline.BeginFrame();
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