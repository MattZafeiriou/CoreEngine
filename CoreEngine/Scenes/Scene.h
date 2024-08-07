#pragma once
#include "../../Objects/CoreObject.h"
#include "../../Objects/LightSources/Light.h"
#include "../../Camera/Camera.h"
#include <map>
#include <vector>

class Scene
{
public:
	Scene(Camera* camera);
	~Scene();
	void AddObject(CoreObject* object);
	void AddLight(Light* light);
	void Render();
private:
	Camera* camera;
	std::vector<CoreObject*> objects;
	std::vector<Light*> lights;
	bool lightUpdate = true;
	int lightCount;
};

