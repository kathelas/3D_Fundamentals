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
#include "CubeSkinScene.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	scenes.push_back( std::make_unique<CubeSkinScene>( gfx, L"pics\\skinnedcube.png") );
	scenes.push_back( std::make_unique<CubeSkinScene>( gfx, L"pics\\skinnedcubemc.png" ) );
	currentScene = scenes.begin();
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
	//const float dt = ft.Mark();
	//
	////changing scene
	//while( !wnd.kbd.KeyIsEmpty() )
	//{
	//	const auto e = wnd.kbd.ReadKey();
	//	if( e.GetCode() == VK_TAB && e.IsPress() )
	//	{
	//		if( wnd.kbd.KeyIsPressed( VK_SHIFT ) )
	//		{
	//			ReverseCycleScene();
	//		}
	//		else
	//		{
	//			CycleScene();
	//		}
	//	}
	//}
	//
	////updating scene
	//(*currentScene)->Update( wnd.kbd, wnd.mouse, dt );




}

void Game::CycleScene()
{
	if( ++currentScene == scenes.end() )
	{
		currentScene = scenes.begin();
	}
}

void Game::ReverseCycleScene()
{
	if( currentScene == scenes.begin() )
	{
		currentScene = scenes.end() - 1;
	}
	else
	{
		--currentScene;
	}
}

void Game::ComposeFrame()
{
	//(*currentScene)->Draw();

	



}