#include "CoreObject.h"

CoreObject::CoreObject(Camera* camera, GLuint VAO, Shader* shader)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	model = glm::mat4(1.0f);
	this->shader = shader;
	this->VAO = VAO;
	this->camera = camera;
}

CoreObject::CoreObject()
{
}

CoreObject::~CoreObject()
{
}

void CoreObject::SetPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

void CoreObject::SetScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
}

void CoreObject::SetRotation(float x, float y, float z)
{
	rotation = glm::vec3(x, y, z);
}

glm::vec3 CoreObject::GetPosition()
{
	return position;
}

glm::vec3 CoreObject::GetScale()
{
	return scale;
}

glm::vec3 CoreObject::GetRotation()
{
	return rotation;
}

glm::mat4 CoreObject::GetModelMatrix()
{
	model = glm::mat4(1.0f);
	if (position != glm::vec3(0.0f, 0.0f, 0.0f))
		model = glm::translate(model, position);
	if (rotation != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (scale != glm::vec3(1.0f, 1.0f, 1.0f))
		model = glm::scale(model, scale);
	return model;
}

void CoreObject::SetShader()
{	
	shader->use();
	if (camera->ShouldUpdate())
	{
		glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)camera->GetWidth() / (float)camera->GetHeight(), camera->closePlane, camera->farPlane);

		shader->setMat4("projection", projection);
	}
	shader->setMat4("view", camera->GetViewMatrix());
}

void CoreObject::Draw()
{
	glBindVertexArray(VAO);
	shader->setMat4("model", GetModelMatrix());
	glDrawArrays(GL_TRIANGLES, 0, 36);
}