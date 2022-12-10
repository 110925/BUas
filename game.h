#pragma once

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
private:
	Surface* screen;
	int mouseX, mouseY, click, release;//mouse variables
	double delx, dely, angle, xvel, yvel, xpyt, ypyt, xypyt, minSpeed = 0.5, maxSpeed = 4; //velocity and angle calculation variables
	int ballX = 300, ballY = 300; //x and y for ballz
};

}; // namespace Tmpl8