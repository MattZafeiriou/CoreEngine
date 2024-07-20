#pragma once
#include "../CoreObject.h"

class Light : public CoreObject
{
public:
	Light(Camera* camera, GLuint VAO, Shader* shader, glm::vec3 color, float intensity);
	Light(Camera* camera, const char* path, Shader* shader, glm::vec3 color, float intensity, bool flip = false);
	Light();
	~Light();
	void Draw();
	void SetColor(glm::vec3 color);
	void SetIntensity(float intensity);
	void SetEnabled(bool enabled);
	glm::vec3 GetColor();
private:
	glm::vec3 color;
	glm::vec3 finalColor;
	float intensity;
	bool enabled;
};

