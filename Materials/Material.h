#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "../Shaders/Shader.h"

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Material
{
public:
	Material(glm::vec3 diffuse, glm::vec3 specular, float shininess, std::vector<Texture> textures);
	Material(glm::vec3 diffuse, glm::vec3 specular, float shininess) : Material(diffuse, specular, shininess, std::vector<Texture>()) {}
	Material();
	~Material();
	void SetDiffuse(glm::vec3 diffuse);
	void SetSpecular(glm::vec3 specular);
	void SetShininess(float shininess);
	void SetMaterial(Shader& shader, bool updateTextures = true, bool updateColor = true);
	void SetTextures(std::vector<Texture> textures);
	glm::vec3 GetDiffuse() { return diffuse; }
	glm::vec3 GetSpecular() { return specular; }
	float GetShininess() { return shininess; }
	std::vector<Texture> GetTextures() { return textures; }
private:
	// Material data
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	std::vector<Texture> textures;
	void SetupTextures(Shader& shader);
};