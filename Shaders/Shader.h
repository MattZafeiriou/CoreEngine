#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int ID; // the shader ID

	Shader(const char* vertexPath, const char* fragmentPath); // constructor
	void use() const; // use the shader

	void setBool(const char* name, bool value) const; // set a boolean uniform
	void setInt(const char* name, int value) const; // set an integer uniform
	void setFloat(const char* name, float value) const; // set a float uniform
	void setVec2(const char* name, glm::vec2 value) const; // set a vec2 uniform
	void setVec3(const char* name, glm::vec3 value) const; // set a vec3 uniform
	void setVec4(const char* name, glm::vec4 value) const; // set a vec4 uniform
	void setMat4(const char* name, glm::mat4 value) const; // set a mat4 uniform
};

