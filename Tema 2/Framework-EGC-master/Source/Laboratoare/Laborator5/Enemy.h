class Enemy
{
public:
	float x;
	float y;
	float angle;
	int angleFall = 0;
	int lives;
	bool outOfRange;
	bool readyToDelete = false;

public:
	void setValues(float enemyX, float enemyY, float enemyAngle, int enemyLives, bool idOutOfRange);
};