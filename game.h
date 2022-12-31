#pragma once
#include <math.h>

namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp( int button ) { release = button; }
	void MouseDown(int button) { click = button; }
	void MouseMove(int x, int y) { mouseX = x, mouseY = y; }
	void KeyUp( int key ) { /* implement if you want to handle keys */ }
	void KeyDown( int key ) { /* implement if you want to handle keys */ }
	double delx, dely, angle, xvel, yvel, xpyt, ypyt, xypyt, minSpeed = 0.5, maxSpeed = 4; //velocity and angle calculation variables
	int ballX = 1280 / 2, ballY = 720 / 2, ballR = 40; //x and y for ballz
	int enemyX = 150, ememyY = -500;
	bool ballsAdded = false;

private:
	Surface* screen;
	int mouseX, mouseY, click, release;//mouse variables
	int gameState = 0; //gameState for menu, game and endscreen
	float shakeIntensity = 8; // intensity of the shake
	int shakeDuration = 0; // duration of the shake, in frames
	int shakeTimer = 0; // make the shake less intense every frame
	int scoreInt = 0;
	bool canShoot = true;

};

}; // namespace Tmpl8