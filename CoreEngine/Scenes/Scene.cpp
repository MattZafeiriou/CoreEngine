#include "Scene.h"

Scene::Scene(Camera* camera)
{
	this->camera = camera;
	lightCount = 0;
}

Scene::~Scene()
{
}

void Scene::AddObject(CoreObject* object)
{
	objects.push_back(object);
}

void Scene::AddLight(Light* light)
{
	lights.push_back(light);
	lightCount++;
	lightUpdate = 1;
}
void Scene::Render()
{
	if (objects.size() == 0)
	{
		return;
	}
	objects[0]->SetShader();
	if (lightUpdate)
	{
		Shader* shader = objects[0]->GetShader();
		shader->setInt("lightCount", lightCount);
		for (int i = 0; i < lightCount; i++)
		{
			shader->setVec3((std::string("pLights[") + std::to_string(i) + std::string("].position")).c_str(), lights[i]->GetPosition());
			shader->setVec4((std::string("pLights[") + std::to_string(i) + std::string("].diffuse")).c_str(), glm::vec4(lights[i]->GetColor(), 1.0f));
			shader->setVec3((std::string("pLights[") + std::to_string(i) + std::string("].specular")).c_str(), lights[i]->GetColor() * glm::vec3(0.5));
			shader->setVec3((std::string("pLights[") + std::to_string(i) + std::string("].ambient")).c_str(), lights[i]->GetColor() * glm::vec3(0.2));
		}
		lightUpdate = 0;
	}
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->Draw();
	}
}