#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Grid
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateGrid(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill = false);

}

