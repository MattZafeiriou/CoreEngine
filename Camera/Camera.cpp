#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(GLFWwindow* window)
{
	this->window = window;

	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	yaw = -90.0f;
	pitch = 0.0f;

	lastX = 400;
	lastY = 300;

	fov = 45.0f;

	deltaTime = 0.0f;
	lastFrame = 0.0f;

	firstMouse = true;

	closePlane = 0.01f;
	farPlane = 100.0f;
	lastClosePlane = closePlane;
	lastFarPlane = farPlane;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	lastW = width;
	lastH = height;

	glm::ortho(0.0f, (float)width, 0.0f, (float)height, closePlane, farPlane);
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void Camera::Update()
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	if (width != lastW || height != lastH || closePlane != lastClosePlane || farPlane != lastFarPlane)
	{
		glm::ortho(0.0f, (float)width, 0.0f, (float)height, closePlane, farPlane);
		lastClosePlane = closePlane;
		lastFarPlane = farPlane;
	}

	// Time between current frame and last frame
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	bool shift = false;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		shift = true;

	// Camera controls
	const float cameraSpeed = 2.5f * deltaTime * (shift+1);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Camera::SetPosition(float x, float y, float z)
{
	cameraPos = glm::vec3(x, y, z);
}

void Camera::SetRotation(float x, float y, float z)
{
	yaw = x;
	pitch = y;
}

int Camera::GetWidth()
{
	return lastW;
}

int Camera::GetHeight()
{
	return lastH;
}

glm::vec3 Camera::GetPosition()
{
	return cameraPos;
}

glm::vec3 Camera::GetRotation()
{
	return glm::vec3(yaw, pitch, 0.0f);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}