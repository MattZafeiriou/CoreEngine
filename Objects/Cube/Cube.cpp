#include "Cube.h"

Cube::Cube(Camera* camera, Shader* shader, glm::vec3 color) : CoreObject(camera, "Resources/Models/Default/cube.obj", shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].color = color;
	}
}

Cube::~Cube()
{
}

void Cube::Draw()
{
	shader->setMat4("model", GetModelMatrix());
	shader->setBool("hasTexture", false);
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(*shader);
	}
}