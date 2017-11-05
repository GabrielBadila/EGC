class Turret
{
public:
	float x;
	float y;
	float z = 0;
	float angle;
	int id;
	double time = 2;
	bool enemyArrive = false;

public:
	void setValues(float turretX, float turretY, float turretAngle, int turretId);
};