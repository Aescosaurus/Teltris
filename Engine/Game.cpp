/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
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
#include "FrameTimer.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	seed( Random::RangeI( 0,100 ) ),
	singleplayer( seed,{ 50,50 },
		{ VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN,
		VK_SPACE,int( 'C' ) },gfx ),
	player1( seed,{ 50,50 },
		{ int( 'A' ),int( 'D' ),int( 'W' ),int( 'S' ),
		VK_SPACE,int( 'C' ) },gfx ),
	player2( seed,{ 450,50 },
		{ VK_LEFT,VK_RIGHT,VK_UP,VK_DOWN,
		VK_BACK,int( 'P' ) },gfx )
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
	const float dt = FrameTimer::Mark();
	
	switch( gameState )
	{
	case State::Menu:
		start1P.Update( wnd.mouse );
		start2P.Update( wnd.mouse );
		if( start1P.IsPressed() )
		{
			gameState = State::Tetris1P;
		}
		else if( start2P.IsPressed() )
		{
			gameState = State::Tetris2P;
		}
		break;
	case State::Tetris1P:
		singleplayer.Update( dt,wnd.kbd );
		break;
	case State::Tetris2P:
		player1.Update( dt,wnd.kbd );
		player2.Update( dt,wnd.kbd );
		break;
	}
}

void Game::ComposeFrame()
{
	switch( gameState )
	{
	case State::Menu:
		start1P.Draw( gfx );
		start2P.Draw( gfx );
		break;
	case State::Tetris1P:
		singleplayer.Draw( gfx );
		break;
	case State::Tetris2P:
		player1.Draw( gfx );
		player2.Draw( gfx );
		break;
	}
}
