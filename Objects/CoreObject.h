#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Shaders/Shader.h"
#include "../Camera/Camera.h"
#include "../Mesh/Mesh.h"
#include "../Materials/Material.h"
#include <vector>

class CoreObject
{
public:
	CoreObject(Camera* camera, GLuint VAO, Shader* shader, Material* material);
	CoreObject(Camera* camera, const char* path, Shader* shader, bool flip = true);
	CoreObject();
	~CoreObject();
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetScale(float scale);
	void SetShader();
	void SetCamera(Camera* camera);
	void Draw(bool updateTextures = true, bool updateColors = false);
	void SetAlwaysPointToCamera(bool alwaysPointToCamera);
	vector<Material*> GetMaterials();
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
	glm::mat4 GetModelMatrix();
	glm::mat4 GetProjectionMatrix();
	glm::vec3 GetMinValues();
	glm::vec3 GetMaxValues();
private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 model;
	glm::vec3 minValues;
	glm::vec3 maxValues;
	GLuint VAO;
	bool alwaysPointToCamera = 0;
	bool isModel;
	bool changedModel;
	vector<Material*> materials;
	std::pair<glm::vec3, glm::vec3> calculateAABB(glm::vec3 minValues, glm::vec3 maxValues, glm::mat4 modelMatrix);
protected:
	Shader* shader;
	Camera* camera;
	vector<Mesh> meshes;
};

