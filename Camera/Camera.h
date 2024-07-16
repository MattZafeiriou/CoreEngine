#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera(GLFWwindow* window);
	void Update();
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void SetPosition(float x, float y, float z);
	void setFov(float fov) { this->fov = fov; }
	void SetRotation(float x, float y, float z);
	float getFov() { return fov; }
	float deltaTime;
	float lastFrame;
	float closePlane;
	float farPlane;
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::mat4 GetViewMatrix();
	int GetWidth();
	int GetHeight();
private:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float fov;
	float lastW;
	float lastH;
	float lastClosePlane;
	float lastFarPlane;
	bool firstMouse;
	GLFWwindow* window;
};

