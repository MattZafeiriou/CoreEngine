#pragma once
#include "../CoreObject.h"

class Plane : public CoreObject
{
public:
	Plane(Camera* camera, Shader* shader, glm::vec4 color = glm::vec4(1.0f));
	Plane() {}
	~Plane();
	void SetColor(glm::vec4 color);
	glm::vec4 GetColor();
	void Draw(bool updateColors = true);
private:
	glm::vec4 color;
};