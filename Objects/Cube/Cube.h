#pragma once
#include "../CoreObject.h"

class Cube : public CoreObject
{
public:
	Cube(Camera* camera, Shader* shader, glm::vec4 color = glm::vec4(1.0f));
	Cube() {}
	~Cube();
	void Draw(bool updateColors = true);
	void SetColor(glm::vec4 color);
	glm::vec4 GetColor();
private:
	glm::vec4 color;
};