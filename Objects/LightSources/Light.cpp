#include "Light.h"

Light::Light(Camera* camera, GLuint VAO, Shader* shader, glm::vec3 color, float intensity) : CoreObject(camera, VAO, shader, new Material(color * intensity, color * 0.5f * intensity, 32.0f))
{
	this->color = color;
	this->intensity = intensity;
	this->finalColor = color * glm::vec3(intensity);
}

Light::Light(Camera* camera, const char* path, Shader* shader, glm::vec3 color, float intensity, bool flip) : CoreObject(camera, path, shader, flip)
{
	this->color = color;
	this->intensity = intensity;
	this->finalColor = color * glm::vec3(intensity);
}

Light::Light()
{}

Light::~Light()
{
}

void Light::SetColor(glm::vec3 color)
{
	this->color = color;
	this->finalColor = color * glm::vec3(intensity);
}

void Light::SetIntensity(float intensity)
{
	this->intensity = intensity;
	this->finalColor = color * glm::vec3(intensity);
}

void Light::SetEnabled(bool enabled)
{
	this->enabled = enabled;
}

glm::vec3 Light::GetColor()
{
	return finalColor;
}

void Light::Draw()
{
	CoreObject::shader->setVec3("lightColor", this->GetColor());
	CoreObject::Draw();
}