#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
#include "Life.h"
#include "Enemy.h"
#include "Turret.h"
#include "Projectile.h"

class Generator : public SimpleScene
{
	public:
		Generator();
		~Generator();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void DrawScene();
		void DrawSceneMini();
		bool CheckRange(float enemyX, float enemyY, int id);
		void FrameEnd() override;
		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;

	protected:
		Laborator::Camera *camera;
		Laborator::Camera *cameraMini;
		glm::mat4 projectionMatrix;
		bool renderCameraTarget;
		int fov;
		int fieldLimit = 12;
		float var;
		float livesRot;
		float playerFixedPosition;
		float numEnemies = 0;
		float color;
		double myTimeEnemy = 5;
		double spamEnemyTime = 5;

		std::vector<Enemy> listEnemies;
		std::vector<Turret> listTurrets;
		std::vector<Projectile> turretsProjectiles;
		Life life1, life2, life3;

		int playerLives = 3;
};
