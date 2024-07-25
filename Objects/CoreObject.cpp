#include "CoreObject.h"
#include "../Mesh/Model.h"
#include "../Objects/Cube/Cube.h"

CoreObject::CoreObject(Camera* camera, GLuint VAO, Shader* shader, Material* material)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	model = glm::mat4(1.0f);
	this->shader = shader;
	this->VAO = VAO;
	this->camera = camera;
	this->isModel = false;
	this->changedModel = true;
	materials.push_back(material);
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
	this->changedModel = true;
	for (int i = 0; i < meshes.size(); i++)
	{
		materials.push_back(meshes[i].GetMaterial());
		if (meshes[i].GetMinValues().x < minValues.x)
			minValues.x = meshes[i].GetMinValues().x;
		if (meshes[i].GetMinValues().y < minValues.y)
			minValues.y = meshes[i].GetMinValues().y;
		if (meshes[i].GetMinValues().z < minValues.z)
			minValues.z = meshes[i].GetMinValues().z;
		if (meshes[i].GetMaxValues().x > maxValues.x)
			maxValues.x = meshes[i].GetMaxValues().x;
		if (meshes[i].GetMaxValues().y > maxValues.y)
			maxValues.y = meshes[i].GetMaxValues().y;
		if (meshes[i].GetMaxValues().z > maxValues.z)
			maxValues.z = meshes[i].GetMaxValues().z;
	}
}

CoreObject::CoreObject()
{
}

CoreObject::~CoreObject()
{
}

glm::vec3 CoreObject::GetMinValues()
{
	return minValues;
}

glm::vec3 CoreObject::GetMaxValues()
{
	return maxValues;
}

void CoreObject::SetPosition(float x, float y, float z)
{
	if (position == glm::vec3(x, y, z))
		return;
	changedModel = true;
	position = glm::vec3(x, y, z);
}

void CoreObject::SetScale(float x, float y, float z)
{
	if (scale == glm::vec3(x, y, z))
		return;
	changedModel = true;
	scale = glm::vec3(x, y, z);
}

void CoreObject::SetScale(float scale)
{
	if (this->scale == glm::vec3(scale, scale, scale))
		return;
	changedModel = true;
	this->scale = glm::vec3(scale, scale, scale);
}

void CoreObject::SetRotation(float x, float y, float z)
{
	if (rotation == glm::vec3(x, y, z))
		return;
	changedModel = true;
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
	if (!changedModel)
		return model;
	changedModel = false;
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

std::pair<glm::vec3, glm::vec3> CoreObject::calculateAABB(glm::vec3 minValues, glm::vec3 maxValues, glm::mat4 modelMatrix)
{
	// Define the 8 corners of the AABB
	glm::vec3 corners[8] = {
		minValues,
		glm::vec3(minValues.x, minValues.y, maxValues.z),
		glm::vec3(minValues.x, maxValues.y, minValues.z),
		glm::vec3(minValues.x, maxValues.y, maxValues.z),
		glm::vec3(maxValues.x, minValues.y, minValues.z),
		glm::vec3(maxValues.x, minValues.y, maxValues.z),
		glm::vec3(maxValues.x, maxValues.y, minValues.z),
		maxValues
	};

	// Transform each corner
	glm::vec3 transformedMin(FLT_MAX);
	glm::vec3 transformedMax(-FLT_MAX);

	for (int i = 0; i < 8; ++i)
	{
		glm::vec4 transformedCorner = modelMatrix * glm::vec4(corners[i], 1.0f);
		transformedMin = glm::min(transformedMin, glm::vec3(transformedCorner));
		transformedMax = glm::max(transformedMax, glm::vec3(transformedCorner));
	}

	return std::make_pair(transformedMin, transformedMax);
}

void CoreObject::Draw(bool updateTextures, bool updateColors)
{
	// Set new position and rotation of the object
	glm::mat4 model = GetModelMatrix();
	shader->setMat4("model", model);

	// If the object is not an imported model, just render the vertices
	if (!isModel){
		materials[0]->SetMaterial(*shader, false, true);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		return;
	}
	// If the object is an imported model, render all the meshes one by one
	// Check if the object is in the camera's frustum
	std::pair<glm::vec3, glm::vec3> transformedBounds = calculateAABB(minValues, maxValues, model);
	glm::vec3 transformedMin = transformedBounds.first;
	glm::vec3 transformedMax = transformedBounds.second;

	bool shouldRender = camera->IsInFrustum(transformedMin, transformedMax);
	if (!shouldRender)
		return;


	//shader->setBool("hasTexture", true);
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(*shader, updateTextures, updateColors);
	}
}

void CoreObject::SetCamera(Camera* camera)
{
	this->camera = camera;
}