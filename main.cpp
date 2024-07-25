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

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
	Assimp::Importer importer;
	setEnvironmentVariables(1); // Set DEBUG to 1
	setEnvironmentVariable("CORE_VERSION", "0.1.2");

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

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Shader shader("Shaders/VertexShaders/test.glsl", "Shaders/FragmentShaders/test.glsl");

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	Shader light("Shaders/VertexShaders/test.glsl", "Shaders/FragmentShaders/light.glsl");

	int diffuse = loadTexture("Resources/Textures/container2.png", 1);
	int specular = loadTexture("Resources/Textures/container2_specular.png", 1);

	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuse);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specular);
	shader.setInt("material.diffuse", 0);
	shader.setInt("material.specular", 1);
	shader.setFloat("material.shininess", 128.0f);

	glm::vec3 lightSourcesPositions[] = {
		glm::vec3(10.0f, 5.0f, 10.0f),
		glm::vec3(10.0f, 5.0f, -10.0f),
		glm::vec3(-10.0f, 5.0f, -10.0f),
		glm::vec3(-10.0f, 5.0f, 10.0f)
	};

	Light lightSources[4];

	for (int i = 0; i < 4; i++)
	{
		lightSources[i] = Light(&camera, lightVAO, &light, glm::vec3(1.0f), 1.0f);
		lightSources[i].SetPosition(lightSourcesPositions[i].x, lightSourcesPositions[i].y, lightSourcesPositions[i].z);
		lightSources[i].SetScale(0.2f, 0.2f, 0.2f);
		lightSources[i].SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3((std::string("pLights[") + std::to_string(i) + std::string("].position")).c_str(), lightSources[i].GetPosition());
		shader.setVec3((std::string("pLights[") + std::to_string(i) + std::string("].diffuse")).c_str(), lightSources[i].GetColor());
		shader.setVec3((std::string("pLights[") + std::to_string(i) + std::string("].specular")).c_str(), lightSources[i].GetColor() * glm::vec3(0.5));
		shader.setVec3((std::string("pLights[") + std::to_string(i) + std::string("].ambient")).c_str(), lightSources[i].GetColor() * glm::vec3(0.2));
	}

	CoreObject bruh(&camera, "Resources/Models/oof/untitled.obj", &shader, 0);
	bruh.SetPosition(0.0f, -2.0f, 0.0f);
	Cube cube(&camera, &shader, glm::vec3(1.0f, 0.5f, 0.31f));

	Plane floor(&camera, &shader);
	floor.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	floor.SetScale(50.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	/*
	 * Render loop
	*/
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// clear the screen
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// renderings
		// light source
		lightSources[0].SetShader();

		for (int i = 0; i < 4; i++)
		{
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
		int max = 20 / 2;
		camera.extractFrustumPlanes();
		for (int i = -max; i < max; i++)
		{
			for (int j = -max; j < max; j++)
			{
				shader.setBool("hasTexture", true);
				bruh.SetPosition(i * 2.0f, 1.0f, j * 2.0f);
				bruh.Draw();
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