#include "Plane.h"

Plane::Plane(Camera* camera, Shader* shader, glm::vec4 color) : CoreObject(camera, "Resources/Models/Default/plane.obj", shader)
{
	SetColor(color);
}

Plane::~Plane()
{
}

glm::vec4 Plane::GetColor()
{
	return color;
}

void Plane::SetColor(glm::vec4 color)
{
	this->color = color;
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].GetMaterial()->SetDiffuse(color);
		meshes[i].GetMaterial()->SetSpecular(color * 0.5f);
	}
}


void Plane::Draw(bool updateColors)
{
	shader->setMat4("model", GetModelMatrix());
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(*shader, false, updateColors);
	}
}