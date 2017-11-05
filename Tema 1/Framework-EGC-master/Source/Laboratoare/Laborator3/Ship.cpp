#include "Ship.h"

void Ship::setValues(float shipX, float shipY, float shipAngle, float shipSpeed, int shipType, int shipLives)
{
	x = shipX;
	y = shipY;
	angle = shipAngle;
	speed = shipSpeed;
	type = shipType;
	lives = shipLives;
}

void Ship::setValues(float shipX, float shipY, float shipAngle, float shipSpeed, int shipLives)
{
	x = shipX;
	y = shipY;
	angle = shipAngle;
	speed = shipSpeed;
	lives = shipLives;
}