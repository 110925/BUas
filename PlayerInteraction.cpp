#include "game.h"
#include "surface.h"
#include "template.h"

namespace Tmpl8 {
	//Player Circle function
	void Game::Circle(Surface* s, float x, float y, float r)
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
	void Game::playerInteraction()
	{
		Health -= 0.3; //Health constantly going down
		screen->Bar(ScreenWidth / 2 - 150, 10, (ScreenWidth / 2 - 150) + Health, 40, 0xff0000); //Show Health
		if (Health <= 0) { //When no more health left... die...
			Kill();
		}

		//On hit with the lava
		if (ballY + ballR > camY + 1000 && ballX < camX || ballY + ballR > camY + 1000 && ballX > camX + 1000) {
			Kill();
		}

		//On hit with the floor;
		if (ballY + ballR > camY + 1000 && ballX > camX && ballX < camX + 1000) {
			yvel *= -0.1;
			yvel--;
			canShoot = true;
		}
	}
}