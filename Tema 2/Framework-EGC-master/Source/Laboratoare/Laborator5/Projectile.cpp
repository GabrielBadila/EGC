#include "Projectile.h"

void Projectile::setValues(float projectileX, float projectileY, float projectileAngle, int turretId)
{
	x = projectileX;
	y = projectileY;
	angle = projectileAngle;
	id = turretId;
}