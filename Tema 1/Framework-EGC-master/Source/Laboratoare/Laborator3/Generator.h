#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include "Ship.h"
#include "Projectile.h"

class Generator : public SimpleScene
{
	public:
		struct ViewportSpace
		{
			ViewportSpace() : x(0), y(0), width(1), height(1) {}
			ViewportSpace(int x, int y, int width, int height)
				: x(x), y(y), width(width), height(height) {}
			int x;
			int y;
			int width;
			int height;
		};

		struct LogicSpace
		{
			LogicSpace() : x(0), y(0), width(1), height(1) {}
			LogicSpace(float x, float y, float width, float height)
				: x(x), y(y), width(width), height(height) {}
			float x;
			float y;
			float width;
			float height;
		};

	public:
		Generator();
		~Generator();

		void Init() override;

		Mesh * CreateMesh(const char * name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void DrawScene(glm::mat3 visMatrix);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

		glm::mat3 VisualizationTransf2DUnif(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);


		void SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

	protected:
		float length;
		const float PI = (float) 3.14159265;
		const float deg2rad = PI / 180;
		float resolutionX = 1280;
		float resolutionY = 720;
		float distCollision = (float) 0.25;
		float scaleGreen = (float) 0.4;
		float scaleBY = (float) 0.46;
		float scaleRed = (float) 0.23;
		double myTime;
		double halfSquare1 = 0.5;
		double halfSquare2 = 0.05;
		bool buttonPressed;
		float color;
		float spamCount;
		float scaleCount;
		float scaleLimit;



		ViewportSpace viewSpace;
		LogicSpace logicSpace;

		Ship player;
		std::vector<Projectile> listProjectiles;
		std::vector<Ship> listEnemies;

		glm::mat3 modelMatrix, visMatrix;
		glm::vec3 red = glm::vec3(1, 0, 0);
		glm::vec3 blue = glm::vec3(0, 0, 1);
		glm::vec3 green = glm::vec3(0, 1, 0);
		glm::vec3 yellow = glm::vec3(1, 1, 0);
		glm::vec3 orange = glm::vec3(1, 0.5, 0);
};
