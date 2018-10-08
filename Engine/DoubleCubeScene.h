#pragma once
#include "Scene.h"
#include "Cube.h"
#include "Pipeline.h"
#include "SolidEffect.h"

class DoubleCubeScene : public Scene
{
public:
	typedef Pipeline<SolidEffect> Pipeline;
	typedef Pipeline::Vertex Vertex;
public:
	DoubleCubeScene( Graphics& gfx )
		:
		itlist( Cube::GetPlainIndependentFaces<Vertex>() ),
		pipeline( gfx )
	{
		const Color colors[] = { Colors::Blue, Colors::Cyan, Colors::Green, Colors::Magenta, Colors::Yellow, Colors::Red };

		for( size_t i = 0; i < itlist.vertices.size(); i++ )
		{
			itlist.vertices[i].color = colors[i / 4];
		}

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
		//fixed cube
		{
			//rotation
			const Mat3 rot =
				Mat3::RotateX( -thetaX ) *
				Mat3::RotateY( -thetaY ) *
				Mat3::RotateZ( -thetaZ );

			//translation
			const Vec3 trans = { 0.0f, 0.0f, 2.0f };

			pipeline.BindRotation( rot );
			pipeline.BindTranslation( trans );

			//render triangles
			pipeline.Draw( itlist );
		}
		
		//mobile cube
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
