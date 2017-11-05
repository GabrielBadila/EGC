#include "Grid.h"

#include <Core/Engine.h>
#include<iostream>

Mesh* Grid::CreateGrid(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	int value = 0;
	int mat[100][100];

	std::vector<VertexFormat> vertices;
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			vertices.push_back(VertexFormat(glm::vec3(i, 0, j), color));
			mat[i][j] = value;
			value++;
		}
	}

	// Umplerea planului
	std::vector<unsigned short> indices;
	for (int i = 0; i < 99; i++) {
		for (int j = 0; j < 99; j++) {
			indices.push_back(mat[i][j]);
			indices.push_back(mat[i][j + 1]);
			indices.push_back(mat[i + 1][j + 1]);

			indices.push_back(mat[i][j]);
			indices.push_back(mat[i + 1][j + 1]);
			indices.push_back(mat[i + 1][j]);
		}
	}

	Mesh* grid = new Mesh(name);
	grid->InitFromData(vertices, indices);
	return grid;
}