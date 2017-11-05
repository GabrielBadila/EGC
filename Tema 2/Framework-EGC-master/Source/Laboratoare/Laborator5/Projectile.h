class Projectile
{
public:
	float x;
	float y;
	float angle;
	float speed = 4;
	int id;

public:
	void setValues(float projectileX, float projectileY, float projectileAngle, int turretId);
};