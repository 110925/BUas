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
		double delx, dely, angle, xvel, yvel, xpyt, ypyt, xypyt, minSpeed = 0.5, maxSpeed = 4; //velocity and angle calculation variables
		int ballX = 1280 / 2, ballY = 720 / 2, ballR = 30; //x and y for ballz
		int enemyX = 150, ememyY = -500;
		bool ballsAdded = false;
		int gameState = 0; //gameState for menu, game and endscreen	
		Surface* screen;


	private:
		int mouseX, mouseY, click, release;//mouse variables
		float shakeIntensity = 8; // intensity of the shake
		int shakeDuration = 0; // duration of the shake, in frames
		int shakeTimer = 0; // make the shake less intense every frame
		int scoreInt = 0;
		bool canShoot = true;
		void Kill();
		void DrawHealth(float x, float y);
		std::vector<Ball> enemies;
		float Health = 300;
	};
}; // namespace Tmpl8