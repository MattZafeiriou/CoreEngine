#include "Light.h"

Light::Light(Camera* camera, GLuint VAO, Shader* shader, glm::vec3 color, float intensity) : CoreObject(camera, VAO, shader)
{
	this->color = color;
	this->intensity = intensity;
}

Light::~Light()
{
}

void Light::SetColor(glm::vec3 color)
{
	this->color = color;
}

glm::vec3 Light::GetColor()
{
	return color * glm::vec3(intensity);
}

void Light::Draw()
{
	CoreObject::SetShader();
	CoreObject::shader->setVec3("lightColor", this->GetColor());
	CoreObject::Draw();
}