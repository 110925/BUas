#include "game.h"
#include "surface.h"
#include <math.h>
#include "template.h"

namespace Tmpl8 {
	void Game::handleControls()
	{
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