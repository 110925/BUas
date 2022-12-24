#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <iostream>
#include <math.h>
#include <SDL2-2.0.3/include/SDL_stdinc.h>
#include "template.h"
#include "windows.h"
#include <vector>
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

	class Ball {

	public:
		float x, y, r;
		float da, db, dc;

		void setVar(float x, float y, float r) {
			this->x = x;
			this->y = y;
			this->r = r;
		}
		void Show(Surface* s,  float x, float y, float r)
		{
			for (int i = 0; i < 64; i++)
			{
				float r1 = (float)i * PI / 32, r2 = (float)(i + 1) * PI / 32;
				s->Line(x - r * sinf(r1), y - r * cosf(r1),
					x - r * sinf(r2), y - r * cosf(r2), 0xff0000);
			}
		}
	};

	void Game::Tick(float deltaTime)
	{
		screen->Clear(0); //clear the graphics window
		Ball ball{};

		ball.r = 20;
		for (int i = 50; i < ScreenWidth; i+=300)
		{
			for (int j = 50; j < ScreenHeight; j+=300)
			{
				ball.setVar( i, j, ball.r);
				ball.Show(screen, ball.x, ball.y, ball.r);
				ball.da = ballX - ball.x;
				ball.db = ballY - ball.y;
				ball.dc = sqrt(ball.da * ball.da + ball.db * ball.db);
				if (ball.dc < ball.r + ballR) {
					if (xvel < 0) {
						ballX+=3;
					}
					if (xvel > 0) {
						ballX-=3;
					}
					if (yvel < 0) {
						ballY+= 3;
					}
					if (yvel > 0) {
						ballX -= 3;
					}
					xvel *= -1;
					yvel *= -1;
				}
			}

		}
		

		//ball.Draw(screen, ballX, ballY); //draw the ball
		Circle(screen, ballX, ballY, ballR);

		if (click && !release) //when mouse is held down
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
			//xpyt = pow(xpyt, 2);
			//ypyt = pow(ypyt, 2);
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
		}

		if (click == 0) //during release
		{
			if (ballX - ballR < 0) {
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
			}
			ballX += xvel;
			ballY += yvel;
		}
		}
	};