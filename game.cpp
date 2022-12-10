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
		// clear the graphics window
		screen->Clear(0);


		// draw a sprite
		ball.Draw(screen, ballX, ballY);
		screen->Line(mouseX, 0, mouseX, 511, 0xff0000);
		screen->Line(0, mouseY, 799, mouseY, 0xff0000);
		if (click && !release) 
		{
			screen->Print("Mouseclick!", 10,10,0xff0000);
			delx = (mouseX - ballX+25);
			dely = (mouseY - ballY+25);
			angle = atan(dely / delx);
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
			screen->Line(ballX+25, ballY+25, mouseX, mouseY, 0xff0000);

		}	
		if (release) 
		{
			click = 0;
			release = 0;
		}				
		if (click == 0) 
		{
			ballX += xvel;
			ballY += yvel;
		}
		
		//std::cout << click; 


	}
};