#pragma once
#include "../CoreObject.h"

class Plane : public CoreObject
{
public:
	Plane(Camera* camera, Shader* shader, glm::vec3 color = glm::vec3(1.0f));
	Plane() {}
	~Plane();
	void Draw();
private:
	glm::vec3 color;
};