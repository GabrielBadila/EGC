#pragma once
#include <vector>
#include <include/glm.h>

class Drop
{
public:
	Drop(glm::vec3 dropPosition);
	
public:
	float scale = 0.25, speed = 0, weight = 0.01, lives = 6;
	glm::vec3 position;
	
public:
	void updatePosition();
};