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
	CoreEngine(int width, int height);
	CoreEngine();
	~CoreEngine();
	void Run();
	void Shutdown();
	void SetCurrentScene(Scene* scene);
	void SetCurrentScene(int index);
	void AddScene(Scene* scene);
	void SetDefaultShader(Shader* shader);
	void SetLightShader(Shader* shader);
	void SetCustomPreRenderFunction(FunctionPointer pointer);
	void SetCustomPostRenderFunction(FunctionPointer pointer);
	void SetPaused(bool paused);
	Scene* GetCurrentScene();
	int GetCurrentSceneIndex();
	int GetTextureColorBuffer();
	Camera* getCamera();
	GLFWwindow* getWindow();
protected:
	void processInput(GLFWwindow* window);
private:
	void CreateFramebuffer(int width, int height);
	void CreateQuadVAO();
	void Render();
	void Init(int width, int height);
	FunctionPointer customPreRenderFunction;
	FunctionPointer customPostRenderFunction;
	GLFWwindow* window;
	Camera* camera;
	Shader* defaultShader;
	Shader* lightShader;
	Shader* framebufferShader;
	std::vector<Scene*> scenes;
	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int quadVAO;
	unsigned int quadVBO;
	unsigned int rbo;
	int currentSceneIndex = 0;
	bool running;
	bool paused = 0;
};

