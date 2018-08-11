/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Tetris1P.h"
#include "Button.h"
#include "HighScoreManager.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	void ResetAll();
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	enum class State
	{
		Menu,
		Tetris1P,
		Tetris2P,
		TetrisVsBot,
		ViewingHighScores
	};
	Font fixedSys = "Fonts/FixedSys16x28.bmp";
	Button backToMenu = Button( { 10,10 },
		"Menu",fixedSys );
	Button start1P = Button( { 50,50 },
		"Play Alone",fixedSys );
	Button start2P = Button( { 50,100 },
		"Play Together",fixedSys );
	Button startVsAI = Button( { 50,150 },
		"Play vs Robots",fixedSys );
	Button highScores = Button( { 50,200 },
		"View High Scores",fixedSys );
	Button quitGame = Button( { 50,250 },
		"Quit",fixedSys );

	State gameState = State::Menu;

	Tetris1P singleplayer;
	Tetris1P player1;
	Tetris1P player2;

	int seed;

	HighScoreManager hsm;
	/********************************/
};