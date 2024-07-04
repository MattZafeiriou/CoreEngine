#ifndef COREWINDOW_CPP
#define COREWINDOW_CPP
#include <cstdlib>
#include "CoreWindow.h"

GLFWwindow* CoreWindow::createWindow(const char* title)
{
    int width = 800;
    int height = 600;

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

#endif // !COREWINDOW_CPP