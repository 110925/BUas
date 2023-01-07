#pragma once
#include <math.h>
#include <vector>

namespace Tmpl8 {
	class Surface;
	class Game;
	class Ball {
		Game* game;
	public:
		float xOrignal, yOrignal, rOrignal;
		float x, y, r;
		float da, db, dc;
		int type, color;
		Ball(Game*, float, float, float);
		void SetNewOffset(float x, float y, float r);
		//Show enemies
		void Show();
		//When platyer hits enemy
		bool Hit();
	};
	class Game
	{
	public:
		void SetTarget(Surface* surface) { screen = surface; }
		void Init();
		void Shutdown();
		void Tick(float deltaTime);
		void MouseUp(int button) { release = button; }
		void MouseDown(int button) { click = button; }
		void MouseMove(int x, int y) { mouseX = x, mouseY = y; }
		void KeyUp(int key) { /* implement if you want to handle keys */ }
		void KeyDown(int key) { /* implement if you want to handle keys */ }
		int ballX = 1280 / 2, ballY = 720 / 2, ballR = 30; //x and y for ballz
		Surface* screen;


	private:		
		void Circle(Surface* s, float x, float y, float r);
		void Kill();		
		void handleControls();
		void playerInteraction();
		void enemyDeletion();
		std::vector<Ball> enemies;

		int mouseX, mouseY, click, release;//mouse variables
		float shakeIntensity = 8; // intensity of the shake
		int shakeDuration = 0; // duration of the shake, in frames
		int shakeTimer = 0; // make the shake less intense every frame
		int gameState = 0; //gameState for menu, game and endscreen	
		bool ballsAdded = false;
		int camX = 150, camY = -550;
		double delx, dely, angle, xvel, yvel, xpyt, ypyt, xypyt, minSpeed = 0.5, maxSpeed = 4; //velocity, angle and max/minSpeed calculation variables
		int scoreInt = 0; //Integer value of the score
		int highScoreInt = 0;//Integer value of the highscore
		bool canShoot = true;
		float Health = 300;
	};
}; // namespace Tmpl8