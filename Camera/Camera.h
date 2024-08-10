#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

struct CamPlane {
	glm::vec3 normal;
	float distance;
};

class Camera
{
public:
	Camera(GLFWwindow* window, bool hideCursor = true);
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
	bool ShouldUpdate();
	bool IsInFrustum(glm::vec3 minValues, glm::vec3 maxValues);
	std::array<CamPlane, 6> extractFrustumPlanes();
private:
	std::array<CamPlane, 6> frustumPlanes;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	float yaw;
	float pitch;
	float lastX;
	float lastY;
	float fov;
	int lastW;
	int lastH;
	float lastClosePlane;
	float lastFarPlane;
	float lastFOV;
	bool firstMouse;
	bool shouldUpdate;
	GLFWwindow* window;
};

