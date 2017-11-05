#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include <vector>
#include "Drop.h"

class Laborator7 : public SimpleScene
{
	public:
		Laborator7();
		~Laborator7();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;

		glm::vec3 lightPosition, dropPosition;
		std::vector<Drop> drops;
		
		unsigned int materialShininess;
		float materialKd;
		float materialKs;

		int pos;
		float waveSpeed = 3, amplitude = 0, waveLength = 4;
		glm::vec3 center;
};
