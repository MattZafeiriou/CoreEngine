#include "Material.h"
#include <glad/glad.h>
#include <iostream>

Material::Material(glm::vec3 diffuse, glm::vec3 specular, float shininess, std::vector<Texture> textures)
{
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
	this->textures = textures;
}

Material::Material()
{
	this->diffuse = glm::vec3(1.0f);
	this->specular = glm::vec3(1.0f);
	this->shininess = 32.0f;
}

Material::~Material()
{
}

void Material::SetTextures(std::vector<Texture> textures)
{
	this->textures = textures;
}

void Material::SetDiffuse(glm::vec3 diffuse)
{
	this->diffuse = diffuse;
}

void Material::SetSpecular(glm::vec3 specular)
{
	this->specular = specular;
}

void Material::SetShininess(float shininess)
{
	this->shininess = shininess;
}

void Material::SetupTextures(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
}

void Material::SetMaterial(Shader& shader, bool updateTextures, bool updateColor)
{
	if (updateColor)
	{
		shader.setVec3("material.diffuseColor", diffuse);
		shader.setVec3("material.specularColor", specular);
		shader.setFloat("material.shininess", shininess);
	}

	if (updateTextures)
	{
		SetupTextures(shader);
	}
}