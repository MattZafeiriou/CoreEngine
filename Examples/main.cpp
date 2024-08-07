#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include "../Window/CoreWindow.h"
#include "../Shaders/Shader.h"
#include "../Camera/Camera.h"
#include "../Objects/CoreObject.h"
#include "../Objects/LightSources/Light.h"
#include <string>
#include "../Objects/Plane/Plane.h"
#include "../Objects/Cube/Cube.h"
#include "../CoreEngine/CoreEngine.h"
#include "../CoreEngine/Scenes/Scene.h"

int main()
{
	CoreEngine coreEngine;
	Camera* camera = coreEngine.getCamera();
	GLFWwindow* window = coreEngine.getWindow();

	Scene* scene = new Scene(camera);
	coreEngine.AddScene(scene);
	coreEngine.SetCurrentScene(0);

	Shader shader("Shaders/VertexShaders/test.glsl", "Shaders/FragmentShaders/test.glsl");
	Shader light("Shaders/VertexShaders/test.glsl", "Shaders/FragmentShaders/light.glsl");

	coreEngine.SetDefaultShader(&shader);
	coreEngine.SetLightShader(&light);

	shader.use();

	glm::vec3 lightSourcesPositions[] = {
		glm::vec3(10.0f, 5.0f, 10.0f),
		glm::vec3(10.0f, 5.0f, -10.0f),
		glm::vec3(-10.0f, 5.0f, -10.0f),
		glm::vec3(-10.0f, 5.0f, 10.0f)
	};

	Light lightSources[4];

	for (int i = 0; i < 4; i++)
	{
		lightSources[i] = Light(camera, "Resources/Models/Default/Cube.obj", &light, glm::vec3(1.0f), 1.0f);
		lightSources[i].SetPosition(lightSourcesPositions[i].x, lightSourcesPositions[i].y, lightSourcesPositions[i].z);
		lightSources[i].SetScale(0.2f);
		lightSources[i].SetColor(glm::vec3(1.0f));
		scene->AddLight(&lightSources[i]);
	}

	CoreObject bruh(camera, "Resources/Models/backpack/backpack.obj", &shader, 0);
	bruh.SetPosition(0.0f, -2.0f, 0.0f);

	Cube cube(camera, &shader, glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));

	Plane floor(camera, &shader);
	floor.SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	floor.SetScale(50.0f);

	//CoreObject test(camera, "Resources/Models/backpack/backpack.obj", &shader, 0);
	//test.SetPosition(2.0f, 0.0f, 0.0f);

	scene->AddObject(&floor);
	scene->AddObject(&cube);
	scene->AddObject(&bruh);
	//scene->AddObject(&test);
	/*
	 * Render loop
	*/
	coreEngine.Run();

	coreEngine.Shutdown();
	return 0;
}