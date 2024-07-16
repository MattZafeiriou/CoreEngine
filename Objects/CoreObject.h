#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Shaders/Shader.h"
#include "../Camera/Camera.h"

class CoreObject
{
public:
	CoreObject(Camera* camera, GLuint VAO, Shader* shader);
	CoreObject();
	~CoreObject();
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetShader();
	void Draw();
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
	glm::mat4 GetModelMatrix();
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 model;
	GLuint VAO;
	Camera* camera;
protected:
	Shader* shader;

};

