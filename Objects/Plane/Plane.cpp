#include "Plane.h"

Plane::Plane(Camera* camera, Shader* shader, glm::vec3 color) : CoreObject(camera, "Resources/Models/Default/plane.obj", shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].color = color;
	}
}

Plane::~Plane()
{
}

void Plane::Draw()
{
	shader->setBool("hasTexture", false);
	shader->setMat4("model", GetModelMatrix());
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(*shader);
	}
}