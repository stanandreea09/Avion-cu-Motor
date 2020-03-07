#include "Tema.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Tema::Tema()
{
}

Tema::~Tema()
{
}

void Tema::Init()
{
	//nor
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//cilindru
	{
		Mesh* mesh = new Mesh("cilindru");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "cilindru.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	


	
	posNor.push_back(2);
	posNor.push_back(2.1);
	posNor.push_back(1.89);
	posNor.push_back(1.75);

	posNor.push_back(3.74);
	posNor.push_back(3.62);
	posNor.push_back(3.59);

	posNor.push_back(5.1);
	posNor.push_back(5.2);
	posNor.push_back(5.32);

	posCombustibil.push_back(2.5);
	posCombustibil.push_back(2.65);
	posCombustibil.push_back(2.8);

	for (int i = 0; i < 7; i++) {
		printf("%d : %f\n", i, posNor[i]);
	}


	//combustibil
	{
		Mesh* mesh = new Mesh("combustibil");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "combustibil.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//obstacole
	{
		Mesh* mesh = new Mesh("obstacol");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "obstacol.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//avion

	{
		Mesh* mesh = new Mesh("avion");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "avionprost.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("elice");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "elice.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Shader* shader = new Shader("Shader");
		shader->AddShader("Source/Tema/Tema/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Tema/Tema/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	fuel = 0.0;
	lives = 3;
	rotatieC = 0.3;
	lightPosition = glm::vec3(0, 1, 1);
	materialShininess = 30;
	materialKd = 0.5;
	materialKs = 0.5;
	rad = 0;
	radNor = 0;
	radA = 0;
	up = false;
	down = false;
	start = false;
	posY = 1.0;
	posObsOy = 0;
	posObsOx = 3.1;
	over = false;
	glm::ivec2 mousepos;
	mousepos = window->GetCursorPosition();
	mouse = mousepos[1];
	rotatieNor = 0.3;
	resolution = window->GetResolution();
	

}

void Tema::FrameStart()
{
	glClearColor(0.7, 0.52, 0.75, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema::Update(float deltaTimeSeconds)
{
	if (start == true && over == false) {
		rotatieC += deltaTimeSeconds * 3;
		rotatieNor += deltaTimeSeconds;
		radNor += deltaTimeSeconds * 4;
		rad += deltaTimeSeconds;
		posObsOx -= deltaTimeSeconds * 0.7;
		posObsOy += deltaTimeSeconds * 0.3;
	}




		//vieti
		for (int i = 0; i < lives; i++) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(-2.2f + i * 0.15, 2.2f, 0);
			modelMatrix *= Transform3D::Scale(0.05f, 0.05f, 0.05f);
			RenderSimpleMesh(meshes["obstacol"], shaders["Shader"], modelMatrix, glm::vec3(0.5f, 0.0f, 0.33f));
		}

		// bara de combustibil
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(1.8f, 2.2f, 0);

			modelMatrix *= Transform3D::Scale(0.9f, 0.1f, 0.01f);

			RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(0.95, 0.61, 0.73));
		}
		{
			glm::mat4 modelMatrix = glm::mat4(1);


			modelMatrix *= Transform3D::Translate(1.75f + (fuel) * 0.5f, 2.17f, 0.1f);

			modelMatrix *= Transform3D::Scale(0.8f - fuel, 0.08f, 0.01f);
			RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(0.7, 0.9, 1));
		}
		if (fuel < 0.8 && start == true) {
			fuel += deltaTimeSeconds * 0.2;
		}

		if (fuel == 0.8 || fuel > 0.8) {
			over = true;
			

		}

		//obstacol
		{
			glm::mat4 modelMatrix = glm::mat4(1);

			//posObs = rand() % 2 + 1;
			modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
			modelMatrix *= Transform3D::RotateOZ(rad * 0.6);
			modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

			modelMatrix *= Transform3D::Translate(1.7f, 1.0f, 0);
			modelMatrix *= Transform3D::RotateOY(rad * 3);
			modelMatrix *= Transform3D::Scale(0.10f, 0.10f, 0.1f);
			RenderSimpleMesh(meshes["obstacol"], shaders["Shader"], modelMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
		}

		//obstacol2
		{
			glm::mat4 modelMatrix = glm::mat4(1);

			modelMatrix *= Transform3D::Translate(posObsOx, posObsOy, 0);
			modelMatrix *= Transform3D::RotateOY(rad * 3);
			modelMatrix *= Transform3D::Scale(0.10f, 0.10f, 0.1f);
			RenderSimpleMesh(meshes["obstacol"], shaders["Shader"], modelMatrix, glm::vec3(1.0f, 0.0f, 0.0f));

		}
		
		if (posObsOx < -4 || posObsOy > 4) {
			posObsOx = 3.1;
			posObsOy = 0;
		}

		//combustibil
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, -2.0f, 1.5f);
			modelMatrix *= Transform3D::RotateOZ(rad * 0.4);
			modelMatrix *= Transform3D::Translate(0, 2.0f, -1.5f);
			modelMatrix *= Transform3D::Translate(2, 1.1f, 0.1f);
			modelMatrix *= Transform3D::RotateOZ(rotatieC);
			modelMatrix *= Transform3D::RotateOX(rotatieC);

			modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			RenderSimpleMesh(meshes["combustibil"], shaders["Shader"], modelMatrix, glm::vec3(0.2f, 1.0f, 0.87f));
		}
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, -2.0f, 1.5f);
			modelMatrix *= Transform3D::RotateOZ(rad * 0.4);
			modelMatrix *= Transform3D::Translate(0, 2.0f, -1.5f);

			modelMatrix *= Transform3D::Translate(2.1f, 1.1f, 0.1f);
			modelMatrix *= Transform3D::RotateOZ(rotatieC);
			modelMatrix *= Transform3D::RotateOX(rotatieC);
			modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			RenderSimpleMesh(meshes["combustibil"], shaders["Shader"], modelMatrix, glm::vec3(0.2f, 1.0f, 0.87f));
		}
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, -2.0f, 1.5f);
			modelMatrix *= Transform3D::RotateOZ(rad * 0.4);
			modelMatrix *= Transform3D::Translate(0, 2.0f, -1.5f);

			modelMatrix *= Transform3D::Translate(2.2f, 1.1f, 0.1f);
			modelMatrix *= Transform3D::RotateOZ(rotatieC);
			modelMatrix *= Transform3D::RotateOX(rotatieC);
			modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			RenderSimpleMesh(meshes["combustibil"], shaders["Shader"], modelMatrix, glm::vec3(0.2f, 1.0f, 0.87f));
		}

		//combustibil2
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, -2.0f, 1.5f);
			modelMatrix *= Transform3D::RotateOZ(rad * 0.4-1);
			modelMatrix *= Transform3D::Translate(0, 2.0f, -1.5f);
			modelMatrix *= Transform3D::Translate(2, 1.1f, 0.1f);
			modelMatrix *= Transform3D::RotateOZ(rotatieC);
			modelMatrix *= Transform3D::RotateOX(rotatieC);

			modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			RenderSimpleMesh(meshes["combustibil"], shaders["Shader"], modelMatrix, glm::vec3(0.2f, 1.0f, 0.87f));
		}
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, -2.0f, 1.5f);
			modelMatrix *= Transform3D::RotateOZ(rad * 0.4-1);
			modelMatrix *= Transform3D::Translate(0, 2.0f, -1.5f);
			modelMatrix *= Transform3D::Translate(2.1, 1.1f, 0.1f);
			modelMatrix *= Transform3D::RotateOZ(rotatieC);
			modelMatrix *= Transform3D::RotateOX(rotatieC);

			modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			RenderSimpleMesh(meshes["combustibil"], shaders["Shader"], modelMatrix, glm::vec3(0.2f, 1.0f, 0.87f));
		}
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, -2.0f, 1.5f);
			modelMatrix *= Transform3D::RotateOZ(rad * 0.4-1);
			modelMatrix *= Transform3D::Translate(0, 2.0f, -1.5f);

			modelMatrix *= Transform3D::Translate(2.2f, 1.1f, 0.1f);
			modelMatrix *= Transform3D::RotateOZ(rotatieC);
			modelMatrix *= Transform3D::RotateOX(rotatieC);
			modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			RenderSimpleMesh(meshes["combustibil"], shaders["Shader"], modelMatrix, glm::vec3(0.2f, 1.0f, 0.87f));
		}
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, -2.0f, 1.5f);
			modelMatrix *= Transform3D::RotateOZ(rad * 0.4-1);
			modelMatrix *= Transform3D::Translate(0, 2.0f, -1.5f);

			modelMatrix *= Transform3D::Translate(2.3f, 1.1f, 0.1f);
			modelMatrix *= Transform3D::RotateOZ(rotatieC);
			modelMatrix *= Transform3D::RotateOX(rotatieC);
			modelMatrix *= Transform3D::Scale(0.1f, 0.1f, 0.1f);
			RenderSimpleMesh(meshes["combustibil"], shaders["Shader"], modelMatrix, glm::vec3(0.2f, 1.0f, 0.87f));
		}
		

		//mare
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
			modelMatrix *= Transform3D::RotateOZ(rad);
			modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);



			modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
			modelMatrix *= Transform3D::RotateOX(1.57f);
			modelMatrix *= Transform3D::Scale(2.0f, 2.0f, 2.0f);
			RenderSimpleMesh(meshes["cilindru"], shaders["Shader"], modelMatrix, glm::vec3(0.65f, 0.7f, 1.0f));
		}

		//Nor
		
		for (int i = 0; i < 13; i++) {
			int n = i % 5;
			switch (n) {
			case 1:
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[0], 1.0f, 0);

				modelMatrix *= Transform3D::RotateOX(rotatieNor);
				modelMatrix *= Transform3D::Scale(0.14f, 0.15f, 0.12f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[1], 1.10f, 0);



				modelMatrix *= Transform3D::RotateOY(0.4);
				modelMatrix *= Transform3D::Scale(0.09f, 0.09f, 0.09f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[2], 1.12f, 0);



				modelMatrix *= Transform3D::Scale(0.11f, 0.12f, 0.12f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			break;

			case 2:
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[0], 1.02f, 0);

				modelMatrix *= Transform3D::RotateOX(rotatieNor);
				modelMatrix *= Transform3D::Scale(0.10f, 0.112f, 0.09f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[1], 1.12f, 0);

				modelMatrix *= Transform3D::RotateOY(0.4);
				modelMatrix *= Transform3D::Scale(0.1f, 0.09f, 0.08f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[2], 1.18f, 0);

				modelMatrix *= Transform3D::Scale(0.11f, 0.11f, 0.12f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			break;
			case 3:
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[1], 1.12f, 0);

				modelMatrix *= Transform3D::RotateOX(rotatieNor);
				modelMatrix *= Transform3D::Scale(0.10f, 0.112f, 0.09f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[2], 1.02f, 0);
				modelMatrix *= Transform3D::RotateOX(0.4);
				modelMatrix *= Transform3D::Scale(0.1f, 0.09f, 0.08f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[3], 1.16f, 0);

				modelMatrix *= Transform3D::Scale(0.11f, 0.11f, 0.12f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			break;

			case 4:
			{
				glm::mat4 modelMatrix = glm::mat4(1);

				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[0], 1.0f, 0);
				modelMatrix *= Transform3D::RotateOX(rotatieNor);
				modelMatrix *= Transform3D::Scale(0.15f, 0.15f, 0.15f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);



				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[1], 1.12f, 0);
				modelMatrix *= Transform3D::RotateOY(0.3);
				modelMatrix *= Transform3D::Scale(0.09f, 0.09f, 0.09f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);


				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[2], 1.05f, 0);

				modelMatrix *= Transform3D::Scale(0.11f, 0.12f, 0.12f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);


				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[3], 1.15f, 0);

				modelMatrix *= Transform3D::RotateOY(rotatieNor);
				modelMatrix *= Transform3D::Scale(0.13f, 0.13f, 0.13f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			break;


			default:
			{
				glm::mat4 modelMatrix = glm::mat4(1);

				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[0], 1.0f, 0);
				modelMatrix *= Transform3D::RotateOX(rotatieNor);
				modelMatrix *= Transform3D::Scale(0.15f, 0.15f, 0.15f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);



				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[1], 1.12f, 0);
				modelMatrix *= Transform3D::RotateOY(0.3);
				modelMatrix *= Transform3D::Scale(0.09f, 0.09f, 0.09f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);


				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[2], 1.05f, 0);

				modelMatrix *= Transform3D::Scale(0.11f, 0.12f, 0.12f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);


				modelMatrix *= Transform3D::Translate(0, -1.5f, 1.5f);
				modelMatrix *= Transform3D::RotateOZ(rad * 0.5 + 0.5 * i);
				modelMatrix *= Transform3D::Translate(0, 1.5f, -1.5f);

				modelMatrix *= Transform3D::Translate(posNor[3], 1.15f, 0);

				modelMatrix *= Transform3D::RotateOY(rotatieNor);
				modelMatrix *= Transform3D::Scale(0.13f, 0.13f, 0.13f);
				RenderSimpleMesh(meshes["box"], shaders["Shader"], modelMatrix, glm::vec3(1.00, 0.72, 0.81));
			}
			}
		}

		// Avion
		
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			if (up == true && over == false && start == true) {
				posY += deltaTimeSeconds * 5;
				up = false;
				if (radA < 0.785) {
					radA += deltaTimeSeconds;
				}
			}
			if (down == true && over == false && start == true) {
				posY -= deltaTimeSeconds * 5;
				//printf("da");
				down = false;
				if (radA > -0.785) {
					radA -= deltaTimeSeconds;
				}

			}
			modelMatrix *= Transform3D::Translate(0, posY, 0);
			modelMatrix *= Transform3D::RotateOZ(radA);
			modelMatrix *= Transform3D::Translate(-0.8f, 0, 0);
			modelMatrix *= Transform3D::Translate(0, 0, 0);
			modelMatrix *= Transform3D::RotateOY(-1.57f);
			modelMatrix *= Transform3D::Scale(0.10f, 0.10f, 0.10f);
			RenderSimpleMesh(meshes["avion"], shaders["Shader"], modelMatrix, glm::vec3(0.75f, 0.45f, 0.61f));
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(0, posY, 0);
			modelMatrix *= Transform3D::RotateOZ(radA);
			modelMatrix *= Transform3D::Translate(-0.8f, 0, 0);
			modelMatrix *= Transform3D::Translate(-0.10f, 0, 0);
			modelMatrix *= Transform3D::RotateOX(rad);
			modelMatrix *= Transform3D::Scale(0.10f, 0.10f, 0.10f);
			modelMatrix *= Transform3D::RotateOZ(3.14f);
			modelMatrix *= Transform3D::RotateOY(1.57f);
			RenderSimpleMesh(meshes["elice"], shaders["Shader"], modelMatrix, glm::vec3(0.84f, 0.59f, 0.71f));
		}

		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, lightPosition);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
		}

		if (lives <= 0) {
			over = true;
		}
		if (over == true) {
			rotatieC =0;
			rotatieNor = 0;
			radNor = 0;
			rad = 0;
			posObsOx = 3.1;
			posObsOy = 0;
		}
		
	
}

void Tema::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Tema::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;
	glUseProgram(shader->program);

	int location = glGetUniformLocation(shader->GetProgramID(), "light_position");
	glUniform3fv(location, 1, glm::value_ptr(lightPosition));

	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	location = glGetUniformLocation(shader->GetProgramID(), "eye_position");
	glUniform3fv(location, 1, glm::value_ptr(eyePosition));
	location = glGetUniformLocation(shader->GetProgramID(), "material_shininess");
	glUniform1i(location, materialShininess);
	location = glGetUniformLocation(shader->GetProgramID(), "material_kd");
	glUniform1f(location, materialKd);
	location = glGetUniformLocation(shader->GetProgramID(), "material_ks");
	glUniform1f(location, materialKs);
	location = glGetUniformLocation(shader->GetProgramID(), "object_color");
	glUniform3fv(location, 1, glm::value_ptr(color));

	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}


void Tema::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

		if (window->KeyHold(GLFW_KEY_W)) lightPosition -= forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_A)) lightPosition -= right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_S)) lightPosition += forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_D)) lightPosition += right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_E)) lightPosition += up * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_Q)) lightPosition -= up * deltaTime * speed;
	}
}

void Tema::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_X) {
		exit(0);
	}
	if (key == GLFW_KEY_SPACE) {
		start = true;
		if (over == true) {
			over = false;
			fuel = 0.0;
		}
	}

}

void Tema::OnKeyRelease(int key, int mods)
{
	
}

void Tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{	
	glm::ivec2 mousepos;
	mousepos = window->GetCursorPosition();

	if (mouse > mousepos[1]) {
		up = true;
	}
	else if (mouse < mousepos[1]) {
		down = true;
	}
	mouse = mousepos[1];
}

void Tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	
}

void Tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	
}

void Tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema::OnWindowResize(int width, int height)
{
}