#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Shaders/Shader.h"
#include "../Camera/Camera.h"
#include "../Mesh/Mesh.h"
#include <vector>

class CoreObject
{
public:
	CoreObject(Camera* camera, GLuint VAO, Shader* shader);
	CoreObject(Camera* camera, const char* path, Shader* shader, bool flip = true);
	CoreObject();
	~CoreObject();
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetScale(float scale);
	void SetShader();
	void SetCamera(Camera* camera);
	void Draw();
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
	glm::mat4 GetModelMatrix();
	glm::mat4 GetProjectionMatrix();
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 model;
	GLuint VAO;
	bool isModel;
protected:
	Shader* shader;
	Camera* camera;
	vector<Mesh> meshes;
};

