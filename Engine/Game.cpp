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
	gfx( wnd ),
	intdist( 0, corners_val - 1 ),
	screen( Graphics::ScreenWidth * Graphics::ScreenHeight, false )
{
	rng.seed( rd() );


	const float angle = (2 * PI) / float( corners_val );
	std::uniform_real_distribution<float> angledist( 0.0f, angle );
	std::uniform_real_distribution<float> lengthdist( 0.5f, 0.9f );


	for( int i = 1; i <= corners_val; i++  )
	{
		//using polar coordinates
		// x = angle, y = length
		const float ang = angledist( rng ) + angle * float(i);
		const float len = lengthdist( rng );

		//transform into cartesian coordinates
		corners.emplace_back( (cos( ang ) * len), (sin( ang ) * len) );
	}
	
	dotPos = corners[0];
	

	//dont want to skip time at the start
	ft.Mark();
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
	auto e = wnd.kbd.ReadKey();
	if( e.IsValid() )
	{
		if( e.IsPress() && e.GetCode() == VK_SPACE )
		{
			if( pause )
			{
				pause = false;
			}
			else
			{
				pause = true;
			}
		}

		if( e.IsPress() && e.GetCode() == 'Q' )
		{
			if( speed < 100 )
			{
				speed++;
			}
		}

		if( e.IsPress() && e.GetCode() == 'A' )
		{
			if( speed > 1 )
			{
				speed--;
			}
		}
	}

	float dt = ft.Mark();
	int count = std::max( int( dt * float(speed * speed_const) ), 1 );
	for( ; count > 0; count-- )
	{
		if( !pause )
		{

			int corn = lastCorn;
			while( corn == lastCorn )
			{
				corn = intdist( rng );
			}

			dotPos = ((corners[corn] - dotPos) * alpha) + dotPos;
			Vec2 screenPos = st.GetTransform( dotPos );
			screen[Graphics::ScreenWidth * int( screenPos.y ) + int( screenPos.x )] = true;

			lastCorn = corn;
		}
	}




}

void Game::ComposeFrame()
{
	for( int i = 0; i < int(screen.size()); i++ )
	{
		if( screen[i] )
		{
			gfx.PutPixel( i % Graphics::ScreenWidth, i / Graphics::ScreenWidth, Colors::White );
		}
	}


	//drawing corners
	for( const auto vec : corners )
	{
		Vec2 v = st.GetTransform( vec );
		gfx.PutPixel( int(v.x), int(v.y), Colors::Yellow );
	}

}