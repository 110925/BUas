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

	void Circle(Surface* s, float x, float y, float r)
	{
		for (int i = 0; i < 64; i++)
		{
			float r1 = (float)i * PI / 32, r2 = (float)(i + 1) * PI / 32;
			s->Line(x - r * sinf(r1), y - r * cosf(r1),
				x - r * sinf(r2), y - r * cosf(r2), 0xffffff);
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
	Ball::Ball(Game* game, float x, float y, float r)
	{
		this->game = game;
		this->xOrignal = this->x = x;
		this->yOrignal = this->y = y;
		this->rOrignal = this->r = r;
	}

	void Ball::SetNewOffset(float xOffset, float yOffset, float rOffset) {
		this->x = xOrignal + xOffset;
		this->y = yOrignal + yOffset;
		this->r = rOrignal + rOffset;
	}

	//Show enemies
	void Ball::Show()
	{
		Surface* s = game->screen;

		//if (type <= 1) {
		//	for (int i = 0; i < 64; i++)
		//	{
		//		float r1 = (float)i * PI / 32, r2 = (float)(i + 1) * PI / 32;
		//		s->Line(x - r * sinf(r1), y - r * cosf(r1),
		//			x - r * sinf(r2), y - r * cosf(r2), 0x00ff00);
		//	}
		//}
		//if (type > 1) {
		for (int i = 0; i < 64; i++)
		{
			float r1 = (float)i * PI / 32, r2 = (float)(i + 1) * PI / 32;
			s->Line(x - r * sinf(r1), y - r * cosf(r1),
				x - r * sinf(r2), y - r * cosf(r2), color);
		}
		//}
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
	void Game::Init()
	{
		for (int i = enemyX-10000; i < enemyX + 10000; i += rand()% 300+250)
		{
			for (int j = ememyY-2000; j < ememyY + 900; j += rand() % 300 +250)
			{
				Ball ball(this, i, j, 15);
				ball.type = rand() % 5 + 1;
				enemies.push_back(ball);
			}
		}
	}
	void Game::Kill()
	{
		gameState = 0;
		enemyX = 150, ememyY = -500;
		yvel = 0, xvel = 0;
		Health = 300;
		scoreInt = 0;
		enemies.clear();
	}
	void Game::DrawHealth(float x, float y) 
	{
		for (int i = 0; i < 20; i++)
		{
			screen->Line(x, y + i, x + Health, y + i, 0xff0000);
		}
	}
	void Game::Tick(float deltaTime)
	{
		screen->Clear(0); //clear the graphics window
		if (gameState == 0) {
			screen->Print("Click anywhere to play!", ScreenWidth / 2 - 100, ScreenHeight / 2 - 100, 0xffffff);
			if (click) {
				gameState = 1; //To the "game" gameState
				canShoot = false;
				if (enemies.size() == 0) {
					Init();
				}
			}
		}
		cout << yvel;
		if (gameState == 1) {

			//DrawHealth(ScreenWidth/2-150,100);
			Health-=0.3; //Health constantly going down
			screen->Bar(ScreenWidth/2 - 150,10, (ScreenWidth / 2 - 150)+Health,40,0xff0000);
			//Detect which enemy is hit and which one to delete
			for (auto it = enemies.begin(); it != enemies.end();)
			{
				auto& ball = *it;
				if (ball.type == 1) {
					ball.color = 0xff0000;
				}
				if(ball.type >= 2){
					ball.color = 0x00ff00;
				}
				if (ball.Hit())
				{

					if (ball.type == 1)
					{
						Kill();
					}

					if (enemies.size() > 0) {
						it = enemies.erase(it);
					}
					else
					{
						it = enemies.begin();
					}

					if (ball.type >= 2)
					{
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
					}
					cout << ball.type;
					scoreInt += 100;
					if (Health < 300) {
						Health += 100;
					}
					canShoot = true;
					shakeDuration = 20;
					shakeTimer = 20;
				}
				else
				{
					++it;
				}

			}

			//Show Enemies
			for (auto& enemy : enemies)
			{
				enemy.SetNewOffset((enemyX - 150), (ememyY + 500), 0);
				enemy.Show();
			}

			//CameraShake
			float shakeOffsetX = shakeIntensity * sin(shakeTimer);
			float shakeOffsetY = shakeIntensity * cos(shakeTimer);
			if (shakeDuration > 0) {
				enemyX += shakeOffsetX;
				ememyY += shakeOffsetY;
				shakeDuration--;
				shakeTimer--;
			}

			screen->Line( -ScreenWidth/2, ememyY + 1000, ScreenWidth , ememyY + 1000, 0xff0000);//lava
			screen->Line(enemyX, ememyY + 1000, enemyX + 1000, ememyY + 1000, 0xffffff);//floor

			//On hit with the floor;
			if (ballY + ballR > ememyY + 1000 && ballX > enemyX && ballX < enemyX + 1000) {
				yvel *= -0.1;
				yvel--;
				canShoot = true;
			}

			//When touch lava
			if (ballY + ballR > ememyY + 1000 && ballX < enemyX || ballY + ballR > ememyY + 1000 && ballX > enemyX + 1000) {
				Kill();
			}

			if (Health <= 0) {
				Kill();
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

				Health -= 0.5;

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
				enemyX -= xvel; //camX
				ememyY -= yvel; //camY
			}
		}
	}
};