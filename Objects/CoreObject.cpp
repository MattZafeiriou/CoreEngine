#include "CoreObject.h"
#include "../Mesh/Model.h"

CoreObject::CoreObject(Camera* camera, GLuint VAO, Shader* shader)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	model = glm::mat4(1.0f);
	this->shader = shader;
	this->VAO = VAO;
	this->camera = camera;
	this->isModel = false;
}

CoreObject::CoreObject(Camera* camera, const char* path, Shader* shader, bool flip)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	model = glm::mat4(1.0f);
	this->shader = shader;
	this->camera = camera;
	this->isModel = true;
	Model model(path, flip);
	meshes = model.GetMeshes();
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

void CoreObject::SetScale(float scale)
{
	this->scale = glm::vec3(scale, scale, scale);
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

glm::mat4 CoreObject::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(camera->getFov()), (float)camera->GetWidth() / (float)camera->GetHeight(), camera->closePlane, camera->farPlane);
}

void CoreObject::SetShader()
{	
	shader->use();
	if (camera->ShouldUpdate())
	{
		glm::mat4 projection = GetProjectionMatrix();
		shader->setMat4("projection", projection);
	}
	shader->setMat4("view", camera->GetViewMatrix());
}

void CoreObject::Draw()
{
	shader->setMat4("model", GetModelMatrix());

	if (!isModel){
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	else
	{
		//shader->setBool("hasTexture", true);
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].Draw(*shader, i == 0, false);
		}
	}
}

void CoreObject::SetCamera(Camera* camera)
{
	this->camera = camera;
}