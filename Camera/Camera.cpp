#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera(GLFWwindow* window, bool hideCursor)
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
	lastFOV = fov;

	deltaTime = 0.0f;
	lastFrame = 0.0f;

	firstMouse = true;

	closePlane = 0.05f;
	farPlane = 100.0f;
	lastClosePlane = closePlane;
	lastFarPlane = farPlane;

	shouldUpdate = true;

	if (hideCursor)
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
	shouldUpdate = false;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	if (width != lastW || height != lastH || closePlane != lastClosePlane || farPlane != lastFarPlane || fov != lastFOV)
	{
		glm::ortho(0.0f, (float)width, 0.0f, (float)height, closePlane, farPlane);
		lastClosePlane = closePlane;
		lastFarPlane = farPlane;
		lastW = width;
		lastH = height;
		lastFOV = fov;
		shouldUpdate = true;
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

bool Camera::ShouldUpdate()
{
	return shouldUpdate;
}

std::array<CamPlane, 6> Camera::extractFrustumPlanes()
{
	glm::mat4 view = GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)lastW / (float)lastH, closePlane, farPlane);
	glm::mat4 viewProjection = projection * view;

	std::array<CamPlane, 6> planes;

	// Extracting planes
	// Left plane
	planes[0].normal.x = viewProjection[0][3] + viewProjection[0][0];
	planes[0].normal.y = viewProjection[1][3] + viewProjection[1][0];
	planes[0].normal.z = viewProjection[2][3] + viewProjection[2][0];
	planes[0].distance = viewProjection[3][3] + viewProjection[3][0];

	// Right plane
	planes[1].normal.x = viewProjection[0][3] - viewProjection[0][0];
	planes[1].normal.y = viewProjection[1][3] - viewProjection[1][0];
	planes[1].normal.z = viewProjection[2][3] - viewProjection[2][0];
	planes[1].distance = viewProjection[3][3] - viewProjection[3][0];

	// Bottom plane
	planes[2].normal.x = viewProjection[0][3] + viewProjection[0][1];
	planes[2].normal.y = viewProjection[1][3] + viewProjection[1][1];
	planes[2].normal.z = viewProjection[2][3] + viewProjection[2][1];
	planes[2].distance = viewProjection[3][3] + viewProjection[3][1];

	// Top plane
	planes[3].normal.x = viewProjection[0][3] - viewProjection[0][1];
	planes[3].normal.y = viewProjection[1][3] - viewProjection[1][1];
	planes[3].normal.z = viewProjection[2][3] - viewProjection[2][1];
	planes[3].distance = viewProjection[3][3] - viewProjection[3][1];

	// Near plane
	planes[4].normal.x = viewProjection[0][2];
	planes[4].normal.y = viewProjection[1][2];
	planes[4].normal.z = viewProjection[2][2];
	planes[4].distance = viewProjection[3][2];

	// Far plane
	planes[5].normal.x = viewProjection[0][3] - viewProjection[0][2];
	planes[5].normal.y = viewProjection[1][3] - viewProjection[1][2];
	planes[5].normal.z = viewProjection[2][3] - viewProjection[2][2];
	planes[5].distance = viewProjection[3][3] - viewProjection[3][2];

	// Normalize the planes
	for (int i = 0; i < 6; i++)
	{
		float length = glm::length(planes[i].normal);
		planes[i].normal /= length;
		planes[i].distance /= length;
	}

	this->frustumPlanes = planes;
	return planes;
}

bool Camera::IsInFrustum(glm::vec3 minValues, glm::vec3 maxValues)
{
	for (int i = 0; i < 6; i++)
	{
		glm::vec3 p = minValues;
		if (frustumPlanes[i].normal.x >= 0)
			p.x = maxValues.x;
		if (frustumPlanes[i].normal.y >= 0)
			p.y = maxValues.y;
		if (frustumPlanes[i].normal.z >= 0)
			p.z = maxValues.z;

		if (glm::dot(frustumPlanes[i].normal, p) + frustumPlanes[i].distance < 0)
			return false;
	}

	return true;
}