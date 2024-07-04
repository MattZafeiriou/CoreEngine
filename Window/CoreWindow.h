#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

class CoreWindow
{
public:
	GLFWwindow* createWindow(const char* title = "CoreEngine");
};

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}