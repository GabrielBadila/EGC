#include "Drop.h"

Drop::Drop(glm::vec3 dropPosition)
{
	position = dropPosition;
}

void Drop::updatePosition()
{
	speed += weight;
	position.y -= speed;
	if (position.y <= 0) {
		lives--;
		position.y = 0;
		scale /= 2;
		weight /= 2;
		speed = -speed / 2;
	}
}