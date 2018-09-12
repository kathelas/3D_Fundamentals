/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Mat3.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	cube( 1.0f )
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = 1.0f / 60.0f;

	if( wnd.kbd.KeyIsPressed( 'Q' ) )
	{
		thetaX = wrapangle( thetaX + dt * dtheta );
	}
	if( wnd.kbd.KeyIsPressed( 'A' ) )
	{
		thetaX = wrapangle( thetaX - dt * dtheta );
	}
	if( wnd.kbd.KeyIsPressed( 'W' ) )
	{
		thetaY = wrapangle( thetaY + dt * dtheta );
	}
	if( wnd.kbd.KeyIsPressed( 'S' ) )
	{
		thetaY = wrapangle( thetaY - dt * dtheta );
	}
	if( wnd.kbd.KeyIsPressed( 'E' ) )
	{
		thetaZ = wrapangle( thetaZ + dt * dtheta );
	}
	if( wnd.kbd.KeyIsPressed( 'D' ) )
	{
		thetaZ = wrapangle( thetaZ - dt * dtheta );
	}
	if( wnd.kbd.KeyIsPressed( 'R' ) )
	{
		zOffset = zOffset + dt;
	}
	if( wnd.kbd.KeyIsPressed( 'F' ) )
	{
		zOffset = zOffset - dt;
	}
}

void Game::ComposeFrame()
{
	auto triangles = cube.GetTriangles();

	//rotation
	const Mat3 rot =
		Mat3::RotateX( thetaX ) *
		Mat3::RotateY( thetaY ) *
		Mat3::RotateZ( thetaZ );

	//transform from model space into "view" space
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
		triangles.cullFlags[i] = ((v1 - v0).VectorProd( v2 - v0 )) * v0 > 0.0f;
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