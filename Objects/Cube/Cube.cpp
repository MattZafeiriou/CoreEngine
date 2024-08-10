#include "Cube.h"

Cube::Cube(Camera* camera, Shader* shader, glm::vec4 color) : CoreObject(camera, "Models/Default/cube.obj", shader)
{
	SetColor(color);
}

Cube::~Cube()
{
}

glm::vec4 Cube::GetColor()
{
	return color;
}

void Cube::SetColor(glm::vec4 color)
{
	this->color = color;
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].GetMaterial()->SetDiffuse(color);
		meshes[i].GetMaterial()->SetSpecular(color * 0.5f);
	}
}

void Cube::Draw(bool updateColors)
{
	shader->setMat4("model", GetModelMatrix());
	//shader->setBool("hasTexture", false);
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(*shader, false, updateColors);
	}
}