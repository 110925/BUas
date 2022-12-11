#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <iostream>
#include <math.h>
#include <SDL2-2.0.3/include/SDL_stdinc.h>
namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
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

	void Game::Tick(float deltaTime)
	{
		screen->Clear(0); //clear the graphics window
		ball.Draw(screen, ballX, ballY); //draw the ball

		if (click && !release) //when mouse is held down
		{
			screen->Print("Mouseclick!", 10, 10, 0xff0000);

			delx = (mouseX - ballX + 25); //measures delta x
			dely = (mouseY - ballY + 25); //measures delta y
			angle = atan(dely / delx); //calculates the angle

			//calculates the correct velocity 
			if (delx >= 0)
			{
				xvel = cos(angle) * 5;
				yvel = sin(angle) * 5;
			}
			else
			{
				xvel = cos(angle) * -5;
				yvel = sin(angle) * -5;
			}

			//calculates the speed using pytagoras 
			if (delx < 0 || dely < 0) {
				xpyt = delx * -1;
				ypyt = dely * -1;
			}
			else
			{
				xpyt = delx;
				ypyt = dely;
			}
			xpyt = pow(xpyt, 2);
			ypyt = pow(ypyt, 2);
			xypyt = sqrt(xpyt + ypyt) / 200;

			//max and min speed
			if (xypyt > maxSpeed) {
				xypyt = maxSpeed;
			}
			if (xypyt < minSpeed) {
				xypyt = minSpeed;
			}
			screen->Line(ballX + 25, ballY + 25, mouseX, mouseY, 0xff0000); //line between ball and mouse
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
			ballX += xvel;
			ballY += yvel;
		}
	}
};