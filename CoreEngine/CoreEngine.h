#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "../Camera/Camera.h"
#include "./Scenes/Scene.h"
#include "../Shaders/Shader.h"
#include "../Objects/CoreObject.h"

class CoreEngine
{
typedef void (*FunctionPointer)();
public:
	/*
	* Creates a new CoreEngine object
	* It initialises the library and creates a window with the specified width and height
	* 
	* 
	* @param width The width of the window
	* @param height The height of the window
	* @param debug Whether to run the engine in debug mode
	*/
	CoreEngine(int width, int height, bool debug = 0);
	/*
	* Creates a new CoreEngine object
	* It initialises the library and creates a window with the default width and height
	* 
	* 
	* @param debug Whether to run the engine in debug mode
	*/
	CoreEngine(bool debug = 0);
	~CoreEngine();
	/*
	* Runs the engine, the rendering process
	* and handles input as well as anything else that is needed for the
	* engine to run
	* 
	* To shut it down manually, call the Shutdown() function or set "running" to false
	*/
	void Run();
	/*
	* Renders the current scene
	 */
	void Render();
	/*
	* Shuts down the engine
	*/
	void Shutdown();
	void SetCurrentScene(Scene* scene);
	void SetCurrentScene(int index);
	void AddScene(Scene* scene);
	void SetDefaultShader(Shader* shader);
	void SetLightShader(Shader* shader);
	void SetCustomRenderFunction(FunctionPointer pointer);
	void SetInputEnabled(bool enabled);
	int GetCurrentSceneIndex();
	Scene* GetCurrentScene();
	Camera* getCamera();
	GLFWwindow* getWindow();
protected:
	void processInput(GLFWwindow* window);
private:
	void Init(int width, int height, bool debug);
	FunctionPointer customRenderFunction;
	GLFWwindow* window;
	Camera* camera;
	Shader* defaultShader;
	Shader* lightShader;
	Shader* framebufferShader;
	std::vector<Scene*> scenes;
	unsigned int framebuffer;
	unsigned int quadVAO;
	unsigned int quadVBO;
	unsigned int rbo;
	int currentSceneIndex = 0;
	bool running;
	bool inputEnabled = true;
};

