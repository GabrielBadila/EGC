#include "Enemy.h"

void Enemy::setValues(float enemyX, float enemyY, float enemyAngle, int enemyLives, bool isOutOfRange)
{
	x = enemyX;
	y = enemyY;
	angle = enemyAngle;
	lives = enemyLives;
	outOfRange = isOutOfRange;
}