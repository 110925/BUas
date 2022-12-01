#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include <iostream>
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
		ball.Draw(screen, 100, 100);
		screen->Line(mouseX, 0, mouseX, 511, 0xff0000);
		screen->Line(0, mouseY, 799, mouseY, 0xff0000);

		if (click && !release) {
			screen->Print("Mouseclick!", 10,10,0xff0000);

		}	
		if (release) {
			click = 0;
			release = 0;
		}		
		
		//std::cout << click; 

	}
};