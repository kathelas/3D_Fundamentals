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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	rng.seed( rd() );


	float angle = (2 * PI) / float( corners_val );
	std::uniform_real_distribution<float> angledist( 0.0f, angle );
	std::uniform_real_distribution<float> lengthdist( 0.5f, 0.9f );


	for( int i = 1; i <= corners_val; i++  )
	{
		//using polar coordinates
		// x = angle, y = length
		float ang = angledist( rng ) * float(i + 1);
		float len = lengthdist( rng );

		//transform into cartesian coordinates
		corners.emplace_back( (cos( ang ) * len), (sin( ang ) * len) );
	}

	screen.reserve( Graphics::ScreenWidth * Graphics::ScreenHeight );

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


}

void Game::ComposeFrame()
{


	//drawing corners
	for( const auto vec : corners )
	{
		Vec2 v = st.GetTransform( vec );
		gfx.PutPixel( v.x, v.y, Colors::Yellow );
	}

}