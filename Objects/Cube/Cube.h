#pragma once
#include "../CoreObject.h"

class Cube : public CoreObject
{
public:
	Cube(Camera* camera, Shader* shader, glm::vec3 color = glm::vec3(1.0f));
	Cube() {}
	~Cube();
	void Draw();
private:
	glm::vec3 color;
};