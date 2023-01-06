#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <iostream>
#include <math.h>
#include <SDL2-2.0.3/include/SDL_stdinc.h>
#include "template.h"
#include "windows.h"
#include <vector>
#include <random>
#include <sstream>

using namespace std;
namespace Tmpl8
{
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	//Player Circle function
	void Circle(Surface* s, float x, float y, float r)
	{
		/****************************************************************************************************************************************
		* Code for this circle can be found at the 3dgep website at part 16 - https://www.3dgep.com/cpp-fast-track-16-physics/#verlet-integration
		* Author: Robert Grigg
		* Date: February 25, 2020
		****************************************************************************************************************************************/
		for (int i = 0; i < 64; i++)
		{
			float r1 = (float)i * PI / 32, r2 = (float)(i + 1) * PI / 32;
			s->Line(x - r * sinf(r1), y - r * cosf(r1),
				x - r * sinf(r2), y - r * cosf(r2), 0xffffff);
		}
	}

	//Class for enemy balls;
	Ball::Ball(Game* game, float x, float y, float r)
	{
		this->game = game;
		this->xOrignal = this->x = x;
		this->yOrignal = this->y = y;
		this->rOrignal = this->r = r;
	}

	//Give the correct offset to the balls
	void Ball::SetNewOffset(float xOffset, float yOffset, float rOffset) {
		this->x = xOrignal + xOffset;
		this->y = yOrignal + yOffset;
		this->r = rOrignal + rOffset;
	}
	//Show enemies
	void Ball::Show()
	{
		Surface* s = game->screen;
		/****************************************************************************************************************************************
		* Code for this circle can be found at the 3dgep website at part 16 - https://www.3dgep.com/cpp-fast-track-16-physics/#verlet-integration
		* Author: Robert Grigg
		* Date: February 25, 2020
		****************************************************************************************************************************************/

		for (int i = 0; i < 64; i++)
		{
			float r1 = (float)i * PI / 32, r2 = (float)(i + 1) * PI / 32;
			s->Line(x - r * sinf(r1), y - r * cosf(r1),
				x - r * sinf(r2), y - r * cosf(r2), color);
		}
	}

	//When platyer hits enemy
	bool Ball::Hit() {
		da = game->ballX - x;
		db = game->ballY - y;
		dc = sqrt(da * da + db * db);
		if (dc < r + game->ballR) {
			return 1;
		}
		return 0;
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


	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		screen->Clear(0); //clear the graphics window

		//Menu
		if (gameState == 0) {
			screen->Print("Click anywhere to play!", ScreenWidth / 2 - 100, ScreenHeight / 2 - 100, 0xffffff);
			
			//Going to the next gameState
			if (click) {
				gameState = 1; 
				canShoot = false;
				if (enemies.size() == 0) {
					Init();
				}
			}

			//Print the highscore
			screen->Print("Your Highscore is:", ScreenWidth / 2 - 100, ScreenHeight / 2 - 130, 0xff0000);
			string str = to_string(highScoreInt);
			char* highScore = const_cast<char*>(str.data());
			screen->Print(highScore, ScreenWidth / 2 +10, ScreenHeight / 2 - 130, 0xff0000);
		}

		//Game
		if (gameState == 1) {

			//Detect which enemy is hit and which one to delete
			for (auto it = enemies.begin(); it != enemies.end();)
			{
				auto& ball = *it;

				if (ball.type <= 3) {//Killer (red) enemy
					ball.color = 0xff0000;
				}
				if(ball.type >= 4 && ball.type <=13) {//Normal (green) enemy
					ball.color = 0x00ff00;
				}
				if (ball.type == 14) {//Golden (gold) enemy
					ball.color = 0xffff00;
				}
				if (ball.type == 15) { //Healer (white) enemy
					ball.color = 0xffffff;
				}

				//When the player hits a ball
				if (ball.Hit())
				{
					//When the player hits the Killer (red) ball
					if (ball.type <= 3)
					{
						Kill();
					}

					//When the player hits the other balls
					if (ball.type >= 4 && ball.type <= 15)
					{
						//Makes Player bounce up on contact with other balls
						if (yvel > 0) {
							yvel *= -1;
							xvel *= 0.7;
							if (yvel < 4) {
								yvel -= 3;
							}
						}
						if (yvel < 0) {
							xvel *= 0.7;
							if (yvel < 4) {
								yvel -= 3;
							}
						}

						//Action for the gold ball
						if (ball.type == 14) 
						{
							scoreInt += 900;
						}

						//Action for the Healer ball
						if (ball.type == 15)
						{
							Health = 300;
						}
					}

					//Should a ball be erased or not?
					if (enemies.size() > 0) {
						it = enemies.erase(it);
					}
					else
					{
						it = enemies.begin();
					}

					scoreInt += 100; //Adds to your score
					if (Health < 300) //Add more health 
					{
						Health += 100;
					}
					canShoot = true; //Make sure the player can shoot again

					//Start the camera shake
					shakeDuration = 20;
					shakeTimer = 20;
				}
				else
				{
					++it;
				}

			}

			//Show Enemies and correct their location by giving them a new offset
			for (auto& enemy : enemies)
			{
				enemy.SetNewOffset((camX - 150), (camY + 500), 0);
				enemy.Show();
			}

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

			//On hit with the floor;
			if (ballY + ballR > camY + 1000 && ballX > camX && ballX < camX + 1000) {
				yvel *= -0.1;
				yvel--;
				canShoot = true;
			}

			//When touch lava
			if (ballY + ballR > camY + 1000 && ballX < camX || ballY + ballR > camY + 1000 && ballX > camX + 1000) {
				Kill();
			}

			Health -= 0.3; //Health constantly going down
			screen->Bar(ScreenWidth / 2 - 150, 10, (ScreenWidth / 2 - 150) + Health, 40, 0xff0000); //Show Health
			if (Health <= 0) { //When no more health left... die...
				Kill();
			}

			//Show the Player ball
			Circle(screen, ballX, ballY, ballR);

			//print an int to the screen by converting it to a char (for the points)
			string str = to_string(scoreInt);
			char* score = const_cast<char*>(str.data());
			screen->Print(score, 10, 10, 0xff0000);//Print the score

			//When mouse is held down
			if (click && !release && canShoot)
			{
				Health -= 0.5; //Health goes down faster

				delx = (mouseX - ballX + ballR / 2); //measures delta x
				dely = (mouseY - ballY + ballR / 2); //measures delta y
				angle = atan2(dely, delx); //calculates the angle

				//calculates the correct velocity 
				xvel = cos(angle) * 5;
				yvel = sin(angle) * 5;

				//calculates the speed using the pythagorean theorem
				if (delx < 0 || dely < 0)
				{
					xpyt = delx * -1;
					ypyt = dely * -1;
				}
				else
				{
					xpyt = delx;
					ypyt = dely;
				}
				xypyt = sqrt(xpyt * xpyt + ypyt * ypyt) / 200;

				//max and min speed
				if (xypyt > maxSpeed)
				{
					xypyt = maxSpeed;
				}
				if (xypyt < minSpeed)
				{
					xypyt = minSpeed;
				}

				screen->Line(ballX + (ballR / PI) / 32, ballY + (ballR / PI) / 32, mouseX, mouseY, 0xff0000); //line between ball and mouse
			}

			if (release) //on release
			{
				click = 0; //Reset the click variable
				release = 0; //Reset the release variable
				xvel *= xypyt; //Add the extra speed
				yvel *= xypyt; //Add the extra speed
				canShoot = false; //Make sure the player cant shoot twice
			}

			if (click == 0 || !canShoot) //during release
			{
				yvel += 0.03; //Gravity
				camX -= xvel; //Reset camX
				camY -= yvel; //Reset camY
			}
		}
	}
};