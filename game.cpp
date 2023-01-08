#include "game.h"
#include "surface.h"
#include "template.h"
#include <cstdio>
#include <random>
#include <sstream>
#include <iostream>

using namespace std;
namespace Tmpl8
{
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		//This spawns the players and gives them random values on what type of ball they are
		for (int i = camX -10000; i < camX + 10000; i += rand()% 300+250)
		{
			for (int j = camY-2000; j < camY + 900; j += rand() % 300 +250)
			{
				Ball ball(this, i, j, 15);
				ball.type = rand() % 15 + 1;
				enemies.push_back(ball);
			}
		}
	}

	//When the player is killed
	void Game::Kill()
	{
		if (scoreInt > highScoreInt)
		{
			highScoreInt = scoreInt;
		}
		gameState = 0;
		camX = 150, camY = -500;
		yvel = 0, xvel = 0;
		Health = 300;
		scoreInt = 0;
		enemies.clear();
	}
	Sprite Logo(new Surface("assets/Bounce_UP.png"), 1);

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		screen->Clear(0); //clear the graphics window
		std::cout << "Timer:" << deltaTime;
		//Menu
		if (gameState == 0) {
			screen->Print("Click anywhere to play!", ScreenWidth / 2 - 100, ScreenHeight / 2 - 60, 0xffffff);
			Logo.Draw(screen, 170, 100);

			//Going to the next gameState
			if (click) {
				gameState = 1; 
				canShoot = false;
				if (enemies.size() == 0) {
					Init();
				}
			}

			//Print the highscore
			screen->Print("Your Highscore is:", ScreenWidth / 2 - 100, ScreenHeight / 2 - 90, 0xff0000);
			string str = to_string(highScoreInt);
			char* highScore = const_cast<char*>(str.data());
			screen->Print(highScore, ScreenWidth / 2 +10, ScreenHeight / 2 - 90, 0xff0000);
		}

		//Game
		if (gameState == 1) {

			//CameraShake
			float shakeOffsetX = shakeIntensity * sin(shakeTimer);
			float shakeOffsetY = shakeIntensity * cos(shakeTimer);
			if (shakeDuration > 0) {
				camX += shakeOffsetX;
				camY+= shakeOffsetY;
				shakeDuration--;
				shakeTimer--;
			}

			screen->Line( -ScreenWidth/2, camY + 1000, ScreenWidth , camY + 1000, 0xff0000);//lava
			screen->Line(camX, camY + 1000, camX + 1000, camY + 1000, 0xffffff);//floor

			//print an int to the screen by converting it to a char (for the points)
			string str = to_string(scoreInt);
			char* score = const_cast<char*>(str.data());
			screen->Print(score, 10, 10, 0xff0000);//Print the score

			//This is the funcion for deleting the correct balls on impact
			enemyDeletion();

			//Show the Player ball
			Circle(screen, ballX, ballY, ballR);

			//This is the function lets the player interact with its 
			playerInteraction();

			//This is the function for all the controls
			handleControls(deltaTime);
		}
	}
};