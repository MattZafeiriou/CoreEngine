#ifndef COREWINDOW_CPP
#define COREWINDOW_CPP
#include <cstdlib>
#include "CoreWindow.h"
#include "../Utils/EnvironmentVariablesUtils.cpp"

/*
 * Initializes GLFW before creating a window
 */
CoreWindow::CoreWindow()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


}

GLFWwindow* CoreWindow::createWindow(const char* title)
{
    int width = 1920;
    int height = 1080;

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // vsync on
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    return window;
}

#endif // !COREWINDOW_CPP