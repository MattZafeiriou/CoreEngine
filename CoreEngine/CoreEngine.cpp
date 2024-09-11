#define COREENGINE_EXPORTS
#include "CoreEngine.h"
#include "../Utils/EnvironmentVariablesUtils.cpp"
#include "../Utils/SystemFileUtils.cpp"
#include "./Framebuffers/Framebuffer.h"
#include "../Window/CoreWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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

CoreEngine::CoreEngine(bool debug)
{
	Init(800, 600, debug);
}

CoreEngine::CoreEngine(int width, int height, bool debug)
{
	Init(width, height, debug);
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

void CoreEngine::Init(int width, int height, bool debug)
{
	setEnvironmentVariables("0.1.6", debug); // Set DEBUG to 1

	const char* assetsPath = getEnvironmentVariable("CORE_ASSETS_PATH");
	//CreateFolder(assetsPath, debug);

	/*
	 * Create a windowed mode window and its OpenGL context
	*/
	CoreWindow coreWindow;
	std::string title = std::string(getEnvironmentVariable("CORE_NAME")) + std::string(" v") + std::string(getEnvironmentVariable("CORE_VERSION"));
	GLFWwindow* window = coreWindow.createWindow(title.c_str(), width, height);
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

void CoreEngine::SetCustomRenderFunction(FunctionPointer pointer)
{
	this->customRenderFunction = pointer;
}

void CoreEngine::Run()
{
	running = true;
	if (defaultShader == nullptr)
	{
		std::cout << "Default shader not set" << std::endl;
		Shutdown();
	}
	// Render loop
	defaultShader->use();
	while (!glfwWindowShouldClose(window) && running)
	{
		// input
		if (inputEnabled)
			processInput(window);
		camera->extractFrustumPlanes();

		if (customRenderFunction != nullptr)
			customRenderFunction();
		else
		{
			glClearColor(1.0, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Render();
		}

		camera->Update();
		glfwSwapBuffers(window);
		// Poll for and process events
		glfwPollEvents();
	}
	glfwTerminate();
}

void CoreEngine::Render()
{
	if (scenes.size() == 0)
	{
		std::cout << "No scenes to render" << std::endl;
		return;
	}
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	defaultShader->use();
	defaultShader->setVec3("viewPos", camera->GetPosition());
 	scenes[currentSceneIndex]->Render();
}

void CoreEngine::Shutdown()
{
	running = false;
	glfwTerminate();
}

void CoreEngine::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
		//glfwSetWindowShouldClose(window, true);

	// mouse click
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void CoreEngine::SetInputEnabled(bool enabled)
{
	inputEnabled = enabled;
}