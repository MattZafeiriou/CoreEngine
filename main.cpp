#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include "Window/CoreWindow.h"
#include "Utils/EnvironmentVariablesUtils.cpp"
#include "Utils/TextureLoader.cpp"
#include "Shaders/Shader.h"
#include "Camera/Camera.h"
#include "Objects/CoreObject.h"
#include "Objects/LightSources/Light.h"
#include <string>
#include "Mesh/Model.h"
#include "Objects/Plane/Plane.h"
#include "Objects/Cube/Cube.h"

static void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->mouse_callback(window, xpos, ypos);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->scroll_callback(window, xoffset, yoffset);
}

int main()
{
	setEnvironmentVariables("0.1.2", 1); // Set DEBUG to 1

	/*
	 * Create a windowed mode window and its OpenGL context
	*/
	CoreWindow coreWindow;
	std::string title = std::string(getEnvironmentVariable("CORE_NAME")) + std::string(" v") + std::string(getEnvironmentVariable("CORE_VERSION"));
	GLFWwindow* window = coreWindow.createWindow(title.c_str());

	Camera camera(window);
	glfwSetWindowUserPointer(window, &camera);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	/*
	 * Initialize GLAD before calling any OpenGL function
	*/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader shader("Shaders/VertexShaders/test.glsl", "Shaders/FragmentShaders/test.glsl");
	Shader light("Shaders/VertexShaders/test.glsl", "Shaders/FragmentShaders/light.glsl");

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
		lightSources[i] = Light(&camera, "Resources/Models/Default/Cube.obj", &light, glm::vec3(1.0f), 1.0f);
		lightSources[i].SetPosition(lightSourcesPositions[i].x, lightSourcesPositions[i].y, lightSourcesPositions[i].z);
		lightSources[i].SetScale(0.2f, 0.2f, 0.2f);
		lightSources[i].SetColor(glm::vec3(1.0f));
		shader.setVec3((std::string("pLights[") + std::to_string(i) + std::string("].position")).c_str(), lightSources[i].GetPosition());
		shader.setVec4((std::string("pLights[") + std::to_string(i) + std::string("].diffuse")).c_str(), glm::vec4(lightSources[i].GetColor(),1.0f));
		shader.setVec3((std::string("pLights[") + std::to_string(i) + std::string("].specular")).c_str(), lightSources[i].GetColor() * glm::vec3(0.5));
		shader.setVec3((std::string("pLights[") + std::to_string(i) + std::string("].ambient")).c_str(), lightSources[i].GetColor() * glm::vec3(0.2));
	}

	CoreObject bruh(&camera, "Resources/Models/bruh/bruh.fbx", &shader);
	bruh.SetPosition(0.0f, -2.0f, 0.0f);
	Cube cube(&camera, &shader, glm::vec3(1.0f, 0.5f, 0.31f));

	Plane floor(&camera, &shader);
	floor.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	floor.SetScale(50.0f);
	/*
	 * Render loop
	*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// clear the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera.extractFrustumPlanes();

		// renderings
		// light source
		lightSources[0].SetShader();
		for (int i = 0; i < 4; i++)
		{
			lightSources[i].SetScale(0.2f);
			lightSources[i].Draw();
		}
		// cube
		floor.SetShader();
		shader.setVec3("viewPos", camera.GetPosition());
		shader.setBool("hasTexture", false);
		floor.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		floor.Draw();

		cube.SetPosition(0.0f, -2.0f, 0.0f);
		cube.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
		cube.Draw(true);
		int max = 10 / 2;

		shader.setBool("hasTexture", 1);
		/*
		Render in order based on the distance from the camera
		*/
		for (int i = -max; i < max; i++)
		{
			for (int j = -max; j < max; j++)
			{
				bruh.SetPosition(i * 2.0f, 1.0f, j * 2.0f);
				bruh.Draw(1,1);
			}
		}
		// check and call events and swap the buffers
		camera.Update(); // update camera and listen events (should be called in the end)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}