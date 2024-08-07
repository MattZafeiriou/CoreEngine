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
	setEnvironmentVariables("0.1.4", 1); // Set DEBUG to 1

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

	CreateQuadVAO();

	framebufferShader = new Shader("Shaders/VertexShaders/framebuffer.glsl", "Shaders/FragmentShaders/framebuffer.glsl");

	CreateFramebuffer();
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
		camera->extractFrustumPlanes();

		Render();

		camera->Update(textureColorbuffer, rbo);

		// Swap the front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}
	glfwTerminate();
}

void CoreEngine::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);
	defaultShader->use();
	defaultShader->setVec3("viewPos", camera->GetPosition());
 	scenes[currentSceneIndex]->Render();

	// second pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	framebufferShader->use();
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CoreEngine::CreateQuadVAO()
{
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void CoreEngine::CreateFramebuffer()
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// generate texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
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