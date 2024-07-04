#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include "Window/CoreWindow.h"
#include "Utils/EnvironmentVariablesUtils.cpp"

static int initialize()
{
    if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for Mac OS X

    setEnvironmentVariable("DEBUG", "1");
    setEnvironmentVariable("CORE_VERSION", "0.0.1");
    return 0;
}


static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    if (initialize() != 0)
        return -1;

    /*
	 * Create a windowed mode window and its OpenGL context
	*/
    CoreWindow coreWindow;
    std::string title = "CoreEngine v" + std::string(getEnvironmentVariable("CORE_VERSION"));
    GLFWwindow* window = coreWindow.createWindow(title.c_str());

    /*
     * Initialize GLAD before calling any OpenGL function
    */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /*
	 * Render loop
	*/
    while(!glfwWindowShouldClose(window))
	{
        // input
        processInput(window);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
	}

    glfwTerminate();
    return 0;
}