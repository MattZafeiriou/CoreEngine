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
	setEnvironmentVariable("CORE_VERSION", "0.1.0");

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

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 lightSourcesPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	Cube cubes[10];
	for (int i = 0; i < 10; i++)
	{
		cubes[i] = Cube(&camera, &shader);
		cubes[i].SetPosition(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
	}

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

	CoreObject bruh(&camera, "Resources/Models/backpack/backpack.obj", &shader, true);
	bruh.SetScale(0.3f);
	bruh.SetPosition(0.0f, -2.0f, 0.0f);

	Plane floor(&camera, &shader);
	floor.SetScale(10.0f);
	/*
	 * Render loop
	*/
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		const float time = glfwGetTime();
		float x = sin(time);
		float y = 2.0f;
		float z = cos(time);
		float radius = 10.0f;
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
		floor.Draw();
		shader.setBool("hasTexture", true);
		bruh.SetPosition(0.0f, -2.0f, 0.0f);
		bruh.Draw();
		for (int i = 1; i <= 10; i++)
		{
			bruh.SetPosition(x * radius / i, y, z * radius / i);
			bruh.Draw();
		}

		// check and call events and swap the buffers
		camera.Update();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}