#pragma once
#include "../CoreObject.h"

class Light : public CoreObject
{
public:
	Light(Camera* camera, GLuint VAO, Shader* shader, glm::vec3 color, float intensity);
	Light();
	~Light();
	void Draw();
	void SetColor(glm::vec3 color);
	glm::vec3 GetColor();
private:
	glm::vec3 color;
	float intensity;
};

