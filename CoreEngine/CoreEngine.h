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
public:
	CoreEngine();
	~CoreEngine();
	void Init();
	void Run();
	void Shutdown();
	void SetCurrentScene(Scene* scene);
	void SetCurrentScene(int index);
	void AddScene(Scene* scene);
	void SetDefaultShader(Shader* shader);
	void SetLightShader(Shader* shader);
	Scene* GetCurrentScene();
	int GetCurrentSceneIndex();
	Camera* getCamera();
	GLFWwindow* getWindow();
protected:
	void processInput(GLFWwindow* window);
private:
	GLFWwindow* window;
	Camera* camera;
	Shader* defaultShader;
	Shader* lightShader;
	std::vector<Scene*> scenes;
	int currentSceneIndex;
	bool running;
};

