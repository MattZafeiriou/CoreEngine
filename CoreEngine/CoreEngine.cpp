#include "CoreEngine.h"
#include "../Utils/EnvironmentVariablesUtils.cpp"
#include "../Window/CoreWindow.h"


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->mouse_callback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
	camera->scroll_callback(window, xoffset, yoffset);
}

Camera* CoreEngine::getCamera()
{
	return camera;
}

GLFWwindow* CoreEngine::getWindow()
{
	return window;
}

CoreEngine::CoreEngine()
{
	Init();
}

CoreEngine::~CoreEngine()
{
}

void CoreEngine::SetDefaultShader(Shader* shader)
{
	defaultShader = shader;
}

void CoreEngine::SetLightShader(Shader* shader)
{
	lightShader = shader;
}

void CoreEngine::Init()
{
	setEnvironmentVariables("0.1.3", 1); // Set DEBUG to 1

	/*
	 * Create a windowed mode window and its OpenGL context
	*/
	CoreWindow coreWindow;
	std::string title = std::string(getEnvironmentVariable("CORE_NAME")) + std::string(" v") + std::string(getEnvironmentVariable("CORE_VERSION"));
	GLFWwindow* window = coreWindow.createWindow(title.c_str());
	this->window = window;

	this->camera = new Camera(window, 1);
	glfwSetWindowUserPointer(window, camera);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Initialize GLAD before calling any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Set the clear color
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void CoreEngine::SetCurrentScene(int index)
{
	if (index < 0 || index >= scenes.size())
	{
		std::cout << "Invalid scene index" << std::endl;
		return;
	}
	currentSceneIndex = index;
}

void CoreEngine::SetCurrentScene(Scene* scene)
{
	for (int i = 0; i < scenes.size(); i++)
	{
		if (scenes[i] == scene)
		{
			currentSceneIndex = i;
			return;
		}
	}
	std::cout << "Scene not found" << std::endl;
}

void CoreEngine::AddScene(Scene* scene)
{
	scenes.push_back(scene);
}

Scene* CoreEngine::GetCurrentScene()
{
	return scenes[currentSceneIndex];
}

int CoreEngine::GetCurrentSceneIndex()
{
	return currentSceneIndex;
}

void CoreEngine::Run()
{
	running = true;
	// Render loop
	defaultShader->use();
	while (!glfwWindowShouldClose(window) && running)
	{
		// input
		processInput(window);

		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera->extractFrustumPlanes();

		// Render the current scene
		defaultShader->use();
		defaultShader->setVec3("viewPos", camera->GetPosition());
		scenes[currentSceneIndex]->Render();

		camera->Update();

		// Swap the front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}
	glfwTerminate();
}

void CoreEngine::Shutdown()
{
	running = false;
}

void CoreEngine::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}