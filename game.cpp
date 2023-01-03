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
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
	}
	void Circle(Surface* s, float x, float y, float r)
	{
		for (int i = 0; i < 64; i++)
		{
			float r1 = (float)i * PI / 32, r2 = (float)(i + 1) * PI / 32;
			s->Line(x - r * sinf(r1), y - r * cosf(r1),
				x - r * sinf(r2), y - r * cosf(r2), 0xff0000);
		}
	}
	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	static Sprite ball(new Surface("assets/ball.png"), 1);
	static int frame = 0;

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------


	//Class for enemy balls;
	class Ball {
		Game game;
	public:
		float x, y, r;
		float da, db, dc;

		void setVar(float x, float y, float r) {
			this->x = x;
			this->y = y;
			this->r = r;
		}
		void Show(Surface* s, float x, float y, float r)
		{
			for (int i = 0; i < 64; i++)
			{
				float r1 = (float)i * PI / 32, r2 = (float)(i + 1) * PI / 32;
				s->Line(x - r * sinf(r1), y - r * cosf(r1),
					x - r * sinf(r2), y - r * cosf(r2), 0xff0000);
			}
		}
		bool Hit(float x, float y, float r) {
			da = game.ballX - x;
			db = game.ballY - y;
			dc = sqrt(da * da + db * db);
			if (dc < r + game.ballR) {
				return 1;
			}
			return 0;
		}
	};	

	void Game::Tick(float deltaTime)
	{
		screen->Clear(0); //clear the graphics window
		Ball ball;
		vector < Ball > enemies;

		if (gameState == 0) {
			screen->Print("Click anywhere to play!", ScreenWidth / 2-100, ScreenHeight/2-100, 0xffffff);
			if (click) {
				gameState = 1; //To the "game" gameState
			}
		}

		if (gameState == 1) {
			ball.r = 20;

			//Enemy location and pushback to vector
			for (int i = enemyX; i < enemyX + 2000; i += 300)
			{
				for (int j = ememyY; j < ememyY + 900; j += 300)
				{
					ball.setVar(i, j, ball.r);
					enemies.push_back(ball);
				}
			}
			//Detect which enemy is hit and which one to delete
			for (auto it = enemies.begin(); it != enemies.end(); ++it)
			{
				auto& ball = *it;

				if (ball.Hit(ball.x, ball.y, ball.r))
				{
					it = enemies.erase(it);
					scoreInt += 100;
					//yvel *= -1;
					//xvel *= -1;
					//if (yvel > 0) {
					//	yvel *= -1.1;
					//}
					//if (yvel < 0) {
					//	yvel *= 1.1;
					//}
					//shakeDuration = 20;
					//shakeTimer = 20;
					canShoot = true;
				}
			}
			//Show Enemies
			for (auto& ball : enemies) ball.Show(screen, ball.x, ball.y, ball.r);

			//CameraShake
			float shakeOffsetX = shakeIntensity * sin(shakeTimer);
			float shakeOffsetY = shakeIntensity * cos(shakeTimer);
			if (shakeDuration > 0) {
				enemyX += shakeOffsetX;
				ememyY += shakeOffsetY;
				shakeDuration--;
				shakeTimer--;
			}

			screen->Line(enemyX-2000, ememyY + 1000, enemyX + 2000, ememyY + 1000, 0xff0000);//floor
			screen->Line(enemyX, ememyY+1000, enemyX+1000, ememyY+1000, 0xffffff);//lava

			//On hit with the floor;
			if (ballY + ballR > ememyY + 1000 && ballX > enemyX && ballX < enemyX+1000) { 
				yvel *= -0.1;
				yvel--;
				canShoot = true;
			}

			//When touch lava
			if (ballY + ballR > ememyY + 1000 && ballX < enemyX || ballY + ballR > ememyY + 1000 && ballX > enemyX + 1000) { 
				gameState = 0;
				enemyX = 150, ememyY = -500;
			}

			//Player
			Circle(screen, ballX, ballY, ballR);

			//print an int to the screen by converting it to a char
			string str = to_string(scoreInt);
			char* score = const_cast<char*>(str.data());
			screen->Print(score, 10, 10, 0xff0000);

			//When mouse is held down
			if (click && !release && canShoot) 
			{
				screen->Print("Mouseclick!", 10, 10, 0xff0000);

				delx = (mouseX - ballX + ballR / 2); //measures delta x
				dely = (mouseY - ballY + ballR / 2); //measures delta y
				angle = atan2(dely, delx); //calculates the angle

				//calculates the correct velocity 
				xvel = cos(angle) * 5;
				yvel = sin(angle) * 5;

				//calculates the speed using pytagoras 
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
				click = 0;
				release = 0;
				xvel *= xypyt;
				yvel *= xypyt;
				canShoot = false;
			}

			if (click == 0 || !canShoot) //during release
			{
				/*if (ballX - ballR < 0) {
					xvel *= -1;
					ballX = 0 + ballR;
				}
				if (ballX + ballR > ScreenWidth) {
					xvel *= -1;
					ballX = ScreenWidth - ballR;
				}
				if (ballY - ballR < 0) {
					yvel *= -1;
					ballY = 0 + ballR;
				}
				if (ballY + ballR > ScreenHeight) {
					yvel *= -1;
					ballY = ScreenHeight - ballR;
				}*/

				yvel += 0.03; //Gravity
				enemyX -= xvel; //cameraMoveX
				ememyY -= yvel; //cameraMoveY
			}
		}
	}
};