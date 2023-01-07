#include "game.h"
#include "surface.h"

namespace Tmpl8 {
	void Game::enemyDeletion() {
		//Detect which enemy is hit and which one to delete
		for (auto it = enemies.begin(); it != enemies.end();)
		{
			auto& ball = *it;

			if (ball.type <= 3) {//Killer (red) enemy
				ball.color = 0xff0000;
			}
			if (ball.type >= 4 && ball.type <= 13) {//Normal (green) enemy
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
	}
}