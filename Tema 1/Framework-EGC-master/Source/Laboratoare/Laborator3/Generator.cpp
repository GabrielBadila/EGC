#include "Generator.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

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
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	cameraInput->SetActive(false);

	// setarea spatiului logic
	logicSpace.x = 0;
	logicSpace.y = 0;
	logicSpace.width = (float) 12.8;
	logicSpace.height = (float) 7.2;
	
	// setarea valorilor initiale ale player-ului
	player.setValues(logicSpace.width / 2, logicSpace.height / 2, 0, 2, 3);

	myTime = 0;
	color = 0;
	spamCount = 0;
	scaleCount = 1;
	scaleLimit = 0;

	vector<VertexFormat> vertices1 =
	{
		VertexFormat(glm::vec3(halfSquare1, -halfSquare1, 0), glm::vec3(0, 1, 0)),
		VertexFormat(glm::vec3(halfSquare1, halfSquare1, 0), glm::vec3(0, 1, 0)),
		VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
		VertexFormat(glm::vec3(-halfSquare1, halfSquare1, 0), glm::vec3(0, 1, 0)),
		VertexFormat(glm::vec3(-halfSquare1, -halfSquare1, 0), glm::vec3(0, 1, 0))
	};

	vector<unsigned short> indices1 =
	{
		0, 1, 2,
		2, 3, 4
	};

	// prima mesha folosita pentru nava player-ului si navele inamice
	Mesh *square1 = CreateMesh("square1", vertices1, indices1);
	AddMeshToList(square1);

	vector<VertexFormat> vertices2 =
	{
		VertexFormat(glm::vec3(halfSquare2, -halfSquare2, 0), glm::vec3(1, 1, 0)),
		VertexFormat(glm::vec3(halfSquare2, halfSquare2, 0), glm::vec3(1, 1, 0)),
		VertexFormat(glm::vec3(-halfSquare2, halfSquare2, 0), glm::vec3(1, 1, 0)),
		VertexFormat(glm::vec3(-halfSquare2, -halfSquare2, 0), glm::vec3(1, 1, 0))
	};

	vector<unsigned short> indices2 =
	{
		0, 1, 2,
		2, 3, 0
	};

	// a doua mesha folosita pentru vieti si pentru proiectile
	Mesh *square2 = CreateMesh("square2", vertices2, indices2);
	AddMeshToList(square2);
}


// Crearea meshelor
Mesh* Generator::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

	glBindVertexArray(0);

	CheckOpenGLError();

	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}


// Matricea de vizualizare
glm::mat3 Generator::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}


// Setarea arenei de vizualizare
void Generator::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	camera->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	camera->Update();
}


// Frame-ul de start
void Generator::FrameStart()
{
	// golirea buffer-ului de culoare folosind culoarea precedenta
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


// Update-ul valorilor in fiecare frame
void Generator::Update(float deltaTimeSeconds)
{

	glm::ivec2 resolution = window->GetResolution();

	// seteaza aria de desenare
	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0), true);
	
	// compune matricea 2D de vizualizare
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

	// daca player-ul are 0 vieti ecranul se blocheaza printr-o animatie
	if (player.lives == 0) {
		color += (float)0.01;
		glClearColor(color, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		player.speed = 0;
		player.angle = 0;
	}
	// altfel update-ul valorilor continua
	else {

		// seteaza coordonatele proiectilelor
		for (int i = 0; i < listProjectiles.size(); i++) {
			listProjectiles[i].x += cos(listProjectiles[i].angle) * deg2rad * listProjectiles[i].speed;
			listProjectiles[i].y += sin(listProjectiles[i].angle) * deg2rad * listProjectiles[i].speed;
		}

		double elapsedTime = Engine::GetElapsedTime();

		// inamicii apar random la un interval de 2 secunde, ce se va decrementa pana la 0.5 secunde
		if (elapsedTime > myTime) {

			if (spamCount <= 15) {
				myTime += 2 - 0.1*spamCount;
				spamCount++;
			}
			else {
				myTime += 0.5;
			}

			Ship enemy;

			int count = rand() / 100;
			int type, lives;

			// spamarea inamicilor in cerc fata de punctul de centru al spatiului logic
			float positionX = sin(2 * PI / 100 * count)*logicSpace.width / 2 + logicSpace.width / 2;
			float positionY = cos(2 * PI / 100 * count)*logicSpace.width / 2 + logicSpace.height / 2;

			if (rand() % 2 == 0) {
				type = 1;
				lives = 1;
			}
			else {
				type = 0;
				lives = 2;
			}

			// calcularea vitezei si a unghiului inamicilor
			float speed = (float) 5 + rand() % (10);
			float angle = atan2(player.y - positionY, player.x - positionX);

			// setarea valorilor inamicilor
			enemy.setValues(positionX, positionY, angle, speed / 10, type, lives);
			listEnemies.push_back(enemy);
		}

		// update-ul valorilor inamicilor (pentru urmarirea player-ului)
		for (int i = 0; i < listEnemies.size(); i++) {
			listEnemies[i].x += cos(listEnemies[i].angle) * deg2rad * listEnemies[i].speed;
			listEnemies[i].y += sin(listEnemies[i].angle) * deg2rad * listEnemies[i].speed;

			float angle = atan2(player.y - listEnemies[i].y, player.x - listEnemies[i].x);
			listEnemies[i].angle = angle;
		}

		int projectilePosition = -1, enemyPosition = -1;

		// verificarea coliziunii dintre inamic si proiectil
		for (int i = 0; i < listProjectiles.size(); i++) {
			for (int j = 0; j < listEnemies.size(); j++) {
				bool collisionX = listProjectiles[i].x + distCollision >= listEnemies.at(j).x &&
					listEnemies.at(j).x + distCollision >= listProjectiles[i].x;
				bool collisionY = listProjectiles[i].y + distCollision >= listEnemies.at(j).y &&
					listEnemies.at(j).y + distCollision >= listProjectiles[i].y;
				if (collisionX && collisionY) {
					listEnemies.at(j).lives--;
					projectilePosition = i;
					enemyPosition = j;
				}
			}
		}

		// tratarea coliziunii dintre inamic si proiectil
		if (projectilePosition != -1 && enemyPosition != -1) {
			if (listEnemies.at(enemyPosition).lives < 1) {
				listEnemies.erase(listEnemies.begin() + enemyPosition);
			}
			else {
				scaleLimit = (float) 0.25 / deltaTimeSeconds; // (250 ns pentru animatia de scalare)
				listEnemies.at(enemyPosition).speed *= 2;
			}
			listProjectiles.erase(listProjectiles.begin() + projectilePosition);
		}


		int enemyPosition2 = -1;

		// verificarea coliziunii dintre inamic si player
		for (int j = 0; j < listEnemies.size(); j++) {
			bool collisionX = player.x + distCollision >= listEnemies.at(j).x &&
				listEnemies.at(j).x + distCollision >= player.x;
			bool collisionY = player.y + distCollision >= listEnemies.at(j).y &&
				listEnemies.at(j).y + distCollision >= player.y;
			if (collisionX && collisionY) {
				player.lives--;
				enemyPosition2 = j;
			}
		}

		// tratarea coliziunii dintre inamic si player
		if (enemyPosition2 != -1) {
			listEnemies.erase(listEnemies.begin() + enemyPosition2);
		}
	}

	DrawScene(visMatrix);
}


// Frame-ul de sfarsit
void Generator::FrameEnd()
{

}


// Desenarea scenei
void Generator::DrawScene(glm::mat3 visMatrix)
{
	// desenarea celor 3 vieti
	if (player.lives == 3) {
		modelMatrix = visMatrix * Transform2D::Translate((float) 11.6, (float) 6.5);
		modelMatrix *= Transform2D::Scale(1, 5);
		RenderMesh2D(meshes["square2"], modelMatrix, yellow);
	}

	if (player.lives >= 2) {
		modelMatrix = visMatrix * Transform2D::Translate((float) 11.8, (float) 6.5);
		modelMatrix *= Transform2D::Scale(1, 5);
		RenderMesh2D(meshes["square2"], modelMatrix, yellow);
	}

	if (player.lives >= 1) {
		modelMatrix = visMatrix * Transform2D::Translate((float) 12.0, (float) 6.5);
		modelMatrix *= Transform2D::Scale(1, 5);
		RenderMesh2D(meshes["square2"], modelMatrix, yellow);
	}

	// desenarea navei player-ului
	modelMatrix = visMatrix * Transform2D::Translate(player.x, player.y);
	modelMatrix *= Transform2D::Rotate(player.angle + PI/2);
	modelMatrix *= Transform2D::Scale(scaleGreen, scaleGreen);
	RenderMesh2D(meshes["square1"], modelMatrix, green);

	// desenarea proiectilelor
	if (buttonPressed == true) {
		for (int i = 0; i < listProjectiles.size(); i++) {
			float angle = atan2(player.y - listProjectiles[i].y, player.x - listProjectiles[i].x);
			modelMatrix = visMatrix * Transform2D::Translate(listProjectiles[i].x, listProjectiles[i].y);
			modelMatrix *= Transform2D::Rotate(angle + PI / 2);
			modelMatrix *= Transform2D::Scale((float) 0.4, (float) 2);
			RenderMesh2D(meshes["square2"], modelMatrix, yellow);
		}
	}

	// desenarea inamicilor
	for (int i = 0; i < listEnemies.size(); i++) {
		float angle = atan2(player.y - listEnemies[i].y, player.x - listEnemies[i].x);
		modelMatrix = visMatrix * Transform2D::Translate(listEnemies[i].x, listEnemies[i].y);
		modelMatrix *= Transform2D::Rotate(angle + PI/2);
		modelMatrix *= Transform2D::Scale(scaleBY, scaleBY);

		if (listEnemies[i].lives == 1 && listEnemies[i].type == 0 || listEnemies[i].type == 2) {
			modelMatrix = visMatrix * Transform2D::Translate(listEnemies[i].x, listEnemies[i].y);
			modelMatrix *= Transform2D::Rotate(angle + PI / 2);
			float coordinate = scaleRed;
			if (listEnemies[i].type == 0) {
				if (scaleLimit != 0 && scaleCount < scaleLimit) {
					coordinate = scaleBY - (scaleRed / scaleLimit) * scaleCount;
					scaleCount++;
				}
				else {
					scaleCount = 1;
					scaleLimit = 0;
					listEnemies[i].type = 2;
				}
			}
			// inamic rosu (mic, o singura viata)
			modelMatrix *= Transform2D::Scale(coordinate, coordinate);
			RenderMesh2D(meshes["square1"], modelMatrix, red);
		}
		else {
			if (listEnemies[i].lives > 0) {
				if (listEnemies[i].type == 1) {
					// inamic albastru (mare, o singura viata)
					RenderMesh2D(meshes["square1"], modelMatrix, blue);
				}
				else {
					// inamic galben (mare, doua vieti)
					RenderMesh2D(meshes["square1"], modelMatrix, yellow);
				}
			}
		}
	}

}


// Controalele de miscare ale navei, dar si zoomIn si zoomOut
void Generator::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) {
		player.y += player.speed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		player.y -= player.speed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		player.x -= player.speed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		player.x += player.speed * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_Q)) {
		logicSpace.width -= player.speed * deltaTime;
		logicSpace.height -= player.speed * deltaTime;
		logicSpace.x += player.speed / 2 * deltaTime;
		logicSpace.y += player.speed / 2 * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_E)) {
		logicSpace.width += player.speed * deltaTime;
		logicSpace.height += player.speed * deltaTime;
		logicSpace.x -= player.speed / 2 * deltaTime;
		logicSpace.y -= player.speed / 2 * deltaTime;
	}
}


void Generator::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// mutarea navei player-ului dupa mouse 
	float coordinateX = mouseX * logicSpace.width / resolutionX - player.x;
	float coordinateY = (logicSpace.height - mouseY * logicSpace.height / resolutionY) - player.y;

	player.angle = atan2f(coordinateY, coordinateX);
};


void Generator::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// lansarea proiectilelor la actionarea butonului stanga al mouse-lui
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {

		buttonPressed = true;
		Projectile bullet;

		float coordinateX = mouseX * logicSpace.width / resolutionX - player.x;
		float coordinateY = (logicSpace.height - mouseY * logicSpace.height / resolutionY) - player.y;

		float angle = atan2f(coordinateY, coordinateX);

		bullet.setValues(player.x, player.y, angle);
		listProjectiles.push_back(bullet);
	};
};
