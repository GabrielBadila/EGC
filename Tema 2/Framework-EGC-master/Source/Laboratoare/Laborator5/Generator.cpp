#include "Generator.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;


Generator::Generator()
{
}


Generator::~Generator()
{
}


// Initializarea valorilor
void Generator::Init()
{
	renderCameraTarget = true;

	// camera principala
	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 0, 2), glm::vec3(0, 0.01f, 0), glm::vec3(0, 1, 0));

	// camera secundara, a minimap-ului
	cameraMini = new Laborator::Camera();
	cameraMini->Set(glm::vec3(0, 0, 2), glm::vec3(0, 0.01f, 0), glm::vec3(0, 1, 0));
	cameraMini->RotateThirdPerson_OX(RADIANS(270));

	playerFixedPosition = 0;
	livesRot = 0;
	color = 0;

	fov = 75;
	var = 10.0f;
	projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);

	// lista turetelor
	Turret turret1, turret2, turret3;
	turret1.setValues(-2, 0, 180.0f, 1);
	turret2.setValues(0, 2, 180.0f, 2);
	turret3.setValues(0, -2, 180.0f, 3);
	listTurrets.push_back(turret1);
	listTurrets.push_back(turret2);
	listTurrets.push_back(turret3);

	// mesha folosita pe minimap
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// mesha folosita pentru reprezentarea vietilor si a proiectilelor
	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// mesha pentru turete
	{
		Mesh* mesh = new Mesh("Turret");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Characters/Turrets", "turret.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// mesha pentru inamici
	{
		Mesh* mesh = new Mesh("Enemy");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Characters/Players", "Enemy.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// mesha pentru player
	{
		Mesh* mesh = new Mesh("Stormtrooper");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Characters/Players", "Stormtrooper.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
}


// Frame-ul de start
void Generator::FrameStart()
{
	// golirea buffer-ului de culoare folosind culoarea precedenta
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();

	// seteaza aria de desenare pentru terenul de joc
	glViewport(0, 0, resolution.x, resolution.y);
	DrawScene();
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);

	// seteaza aria de desenare pentru minimap
	glm::mat4 projectionMatrixMini = glm::ortho(-12.0f, 12.0f, -12.0f, 12.0f, 0.1f, 250.0f);
	glViewport(resolution.x / 1.235f, 0, resolution.y / 3, resolution.y / 3);
	DrawSceneMini();
	DrawCoordinatSystem(cameraMini->GetViewMatrix(), projectionMatrixMini);
}


// Update-ul valorilor in fiecare frame
void Generator::Update(float deltaTimeSeconds)
{	
	// daca player-ul are 0 vieti ecranul se blocheaza printr-o animatie
	if (playerLives == 0) {
		color += (float)0.01;
		glClearColor(color, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	// altfel update-ul valorilor continua
	else {

		double elapsedTime = Engine::GetElapsedTime();
		int enemyPositionOut = -1, projectilePositionOut = -1;
		int enemyPosition = -1, projectilePosition = -1;

		// miscarea inamicilor in Z
		for (int i = 0; i < listEnemies.size(); i++) {
			if (listEnemies[i].lives > 0) {
				if ((listEnemies[i].x == -fieldLimit && listEnemies[i].y < fieldLimit) || (listEnemies[i].x > fieldLimit && listEnemies[i].y < fieldLimit)) {
					listEnemies[i].y += 0.04;
					listEnemies[i].angle = 90;
				}
				else {
					listEnemies[i].y -= 0.04;
					listEnemies[i].x += 0.04;
					listEnemies[i].angle = 315;
				}

				if (listEnemies[i].x > fieldLimit && listEnemies[i].y > fieldLimit) {
					listEnemies[i].outOfRange = true;
				}
			}
		}

		// sterge inamicii in urma coliziunilor dupa ce au realizat rotatie de cazatura
		for (int i = 0; i < listEnemies.size(); i++) {
			if (listEnemies[i].readyToDelete == true) {
				enemyPosition = i;
			}
		}

		if (enemyPosition != -1) {
			listEnemies.erase(listEnemies.begin() + enemyPosition);
		}

		// rotirea turetelor si crearea proiectilelor atunci cand un inamic a patruns in range-ul lor
		for (int i = 0; i < listTurrets.size(); i++) {
			for (int j = 0; j < listEnemies.size(); j++) {
				if (CheckRange(listEnemies[j].x, listEnemies[j].y, listTurrets[i].id)) {
					listTurrets[i].angle = atan2f(listEnemies[j].y - listTurrets[i].y, listEnemies[j].x - listTurrets[i].x);
					listTurrets[i].angle = listTurrets[i].angle * 180 / 3.14;

					// setarea contorului atunci cand primul inamic a patruns in range
					if (listTurrets[i].enemyArrive == false) {
						listTurrets[i].enemyArrive = true;
						listTurrets[i].time = elapsedTime;
					}

					if (elapsedTime >= listTurrets[i].time) {
						listTurrets[i].time += 2;
						Projectile bullet;

						float coordinateX = listEnemies[j].x - listTurrets[i].x;
						float coordinateY = listEnemies[j].y - listTurrets[i].y;

						float angle = atan2f(coordinateY, coordinateX);

						bullet.setValues(listTurrets[i].x, listTurrets[i].y, angle, listTurrets[i].id);
						turretsProjectiles.push_back(bullet);
					}

					break;
				}
			}
		}

		// modificarea coordonatelor proiectilelor si stergerea celor care au iesit din range
		for (int i = 0; i < turretsProjectiles.size(); i++) {
			turretsProjectiles[i].x += cos(turretsProjectiles[i].angle) / 15;
			turretsProjectiles[i].y += sin(turretsProjectiles[i].angle) / 15;

			if (CheckRange(turretsProjectiles[i].x, turretsProjectiles[i].y, turretsProjectiles[i].id) == false) {
				projectilePositionOut = i;
			}

		}

		if (projectilePositionOut != -1) {
			turretsProjectiles.erase(turretsProjectiles.begin() + projectilePositionOut);
		}

		float distCollision = 0.5;

		// verificarea coliziunii dintre inamic si proiectil
		for (int i = 0; i < turretsProjectiles.size(); i++) {
			for (int j = 0; j < listEnemies.size(); j++) {
				bool collisionX = turretsProjectiles[i].x + distCollision >= listEnemies.at(j).x &&
					listEnemies.at(j).x + distCollision >= turretsProjectiles[i].x;
				bool collisionY = turretsProjectiles[i].y + distCollision >= listEnemies.at(j).y &&
					listEnemies.at(j).y + distCollision >= turretsProjectiles[i].y;
				if (collisionX && collisionY) {
					listEnemies.at(j).lives--;
					projectilePosition = i;
				}
			}
		}

		// tratarea coliziunii pentru proiectile
		if (projectilePosition != -1) {
			turretsProjectiles.erase(turretsProjectiles.begin() + projectilePosition);
		}


		// stergere inamic iesit din terenul de joc
		for (int i = 0; i < listEnemies.size(); i++) {
			if (listEnemies[i].outOfRange == true) {
				enemyPositionOut = i;
				break;
			}
		}

		// stergerea propriu-zisa si decrementarea numarului de vieti al player-ului
		if (enemyPositionOut != -1) {
			playerLives--;
			listEnemies.erase(listEnemies.begin() + enemyPositionOut);
		}


		// spamare inamici
		if (elapsedTime > myTimeEnemy) {
			myTimeEnemy += spamEnemyTime;
			int lives = 2;

			// inamici mai rapizi si mai puternici
			if (numEnemies > 2) {
				if (numEnemies > 5) {
					if (numEnemies > 7) {
						spamEnemyTime = 2;
						lives += 6;
					}
					else {
						spamEnemyTime = 3;
						lives += 4;
					}
				}
				else {
					spamEnemyTime = 4;
					lives += 2;
				}
			}

			Enemy enemy;
			enemy.setValues(-fieldLimit, -fieldLimit, 90, lives, false);
			listEnemies.push_back(enemy);
			numEnemies++;
		}

		// modificarea constantei de rotire a vietilor
		livesRot += 2 * deltaTimeSeconds;
	}
}

// Desenarea scenei jocului
void Generator::DrawScene()
{
	// desenarea inamicilor + rotatia de cazatura
	for (int i = 0; i < listEnemies.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(listEnemies[i].y, 0.9f, listEnemies[i].x));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(listEnemies[i].angle), glm::vec3(0, 1, 0));

		if (listEnemies[i].lives < 1) {
			listEnemies[i].angleFall += 1;
			modelMatrix = glm::rotate(modelMatrix, RADIANS(listEnemies[i].angleFall), glm::vec3(0, 0, 1));

			if (listEnemies[i].angleFall == 90) {
				listEnemies[i].readyToDelete = true;
			}
		}

		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7f));
		RenderMesh(meshes["Enemy"], shaders["VertexNormal"], modelMatrix);
	}

	// desenarea proiectilelor turetelor
	for (int i = 0; i < turretsProjectiles.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(turretsProjectiles[i].y, 0.5, turretsProjectiles[i].x));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
	}

	// desenarea turetelor
	for (int i = 0; i < listTurrets.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(listTurrets[i].y, listTurrets[i].z, listTurrets[i].x));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(listTurrets[i].angle), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		RenderMesh(meshes["Turret"], shaders["VertexNormal"], modelMatrix);
	}

	// rotatia celor 3 vieti in jurul player-ului

	if (playerLives == 3) {
		glm::mat4 modelMatrix = glm::mat4(1);
		life1.setValues(camera->GetTargetPosition().x, camera->GetTargetPosition().y - 0.1, camera->GetTargetPosition().z);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(life1.x, life1.y, life1.z));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0.05));
		modelMatrix = glm::rotate(modelMatrix, playerFixedPosition, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, livesRot, glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, -0.05));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
	}
	
	if (playerLives >= 2) {
		glm::mat4 modelMatrix = glm::mat4(1);
		life2.setValues(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 0.4, camera->GetTargetPosition().z - 0.4);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(life2.x, life2.y, life2.z));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0.4));
		modelMatrix = glm::rotate(modelMatrix, playerFixedPosition, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, livesRot, glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -0.4));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
	}

	if (playerLives >= 1) {
		glm::mat4 modelMatrix = glm::mat4(1);
		life3.setValues(camera->GetTargetPosition().x - 0.05, camera->GetTargetPosition().y - 0.1, camera->GetTargetPosition().z);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(life3.x, life3.y, life3.z));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.05, 0.5, 0));
		modelMatrix = glm::rotate(modelMatrix, playerFixedPosition, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, livesRot, glm::vec3(0, 0, 1));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.05, -0.5, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
	}

	// desenarea player-ului folosind positia camerei Third-Person
	if (renderCameraTarget) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
		modelMatrix = glm::rotate(modelMatrix, playerFixedPosition, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(180.0f), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
		RenderMesh(meshes["Stormtrooper"], shaders["VertexNormal"], modelMatrix);
	}
}


// Desenarea scenei minimap-ului
void Generator::DrawSceneMini()
{
	// desenarea inamicilor sub forma de box-uri
	for (int i = 0; i < listEnemies.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(listEnemies[i].y, 0.9f, listEnemies[i].x));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(listEnemies[i].angle), glm::vec3(0, 1, 0));

		if (listEnemies[i].lives < 1) {
			listEnemies[i].angleFall += 1;
			modelMatrix = glm::rotate(modelMatrix, RADIANS(listEnemies[i].angleFall), glm::vec3(0, 0, 1));

			if (listEnemies[i].angleFall == 90) {
				listEnemies[i].readyToDelete = true;
			}
		}

		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7f));
		RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	}

	// desenarea turetelor sub forma de box-uri
	for (int i = 0; i < listTurrets.size(); i++) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(listTurrets[i].y, listTurrets[i].z, listTurrets[i].x));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(listTurrets[i].angle), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	}

	// desenarea player-ului sub forma de box
	if (renderCameraTarget) {
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
		modelMatrix = glm::rotate(modelMatrix, playerFixedPosition, glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(180.0f), glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
		RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
	}
}


// Verifica daca inamicii si proiectilele se afla in range-ul turetelor
bool Generator::CheckRange(float enemyX, float enemyY, int id) {
	if (id == 1 || id == 2) {
		if ((enemyX >= -4 && enemyX <= 2) && (enemyY >= -2 && enemyY <= 4)) {
			return true;
		}
	}
	else if (id == 3) {
		if ((enemyX >= -2 && enemyX <= 4) && (enemyY >= -4 && enemyY <= 2)) {
			return true;
		}
	}

	return false;
}


// Frame-ul de sfarsit
void Generator::FrameEnd()
{
	
}


// Functia de randare a meshelor
void Generator::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader)
		return;

	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, false, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}


// Controalele de miscare ale player-ului, dar si zoomIn si zoomOut pentru ambele camere
void Generator::OnInputUpdate(float deltaTime, int mods)
{
	float cameraSpeed = 2.0f;

	if (window->KeyHold(GLFW_KEY_W)) {
		camera->MoveForward(cameraSpeed * deltaTime);
		cameraMini->MoveForward(cameraSpeed * deltaTime);
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		camera->TranslateRight(-cameraSpeed * deltaTime);
		cameraMini->TranslateRight(-cameraSpeed * deltaTime);
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		camera->MoveForward(-cameraSpeed * deltaTime);
		cameraMini->MoveForward(-cameraSpeed * deltaTime);
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		camera->TranslateRight(cameraSpeed * deltaTime);
		cameraMini->TranslateRight(cameraSpeed * deltaTime);
	}

	if (window->KeyHold(GLFW_KEY_Q)) {
		camera->TranslateUpword(-cameraSpeed * deltaTime);
	}

	if (window->KeyHold(GLFW_KEY_E)) {
		camera->TranslateUpword(cameraSpeed * deltaTime);
	}

	if (window->KeyHold(GLFW_KEY_Z)) {
		fov += deltaTime * 100;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (window->KeyHold(GLFW_KEY_X)) {
		fov -= deltaTime * 100;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}
}


// Controalele de schimbare a armelor
void Generator::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_T) {
		renderCameraTarget = !renderCameraTarget;
	}

	if (key == GLFW_KEY_1) {
		fov = 75;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (key == GLFW_KEY_2) {
		fov = 30;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}

	if (key == GLFW_KEY_3) {
		fov = 75;
		projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	}
}


// Camera Third-Person si calcularea unghiului de miscare al player-ului pentru a-l pastra cu spatele la camera
void Generator::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = true;
			camera->RotateThirdPerson_OX(deltaY * sensivityOY);
			camera->RotateThirdPerson_OY(deltaX * sensivityOX);
		}
		playerFixedPosition += deltaX * sensivityOX;
	}
}