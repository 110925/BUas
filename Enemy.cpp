#include "game.h"
#include "surface.h"
#include "template.h"
//#include <math.h>

namespace Tmpl8 
{
	//Class for enemy balls;
	Ball::Ball(Game* game, float x, float y, float r)
	{
		this->game = game;
		this->xOrignal = this->x = x;
		this->yOrignal = this->y = y;
		this->rOrignal = this->r = r;
	}

	//Give the correct offset to the balls
	void Ball::SetNewOffset(float xOffset, float yOffset, float rOffset) 
	{
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
		if (dc < r + game->ballR) 
		{
			return 1;
		}
		return 0;
	}
}