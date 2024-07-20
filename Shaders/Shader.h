#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

class Shader
{
public:
	unsigned int ID; // the shader ID

	Shader(const char* vertexPath, const char* fragmentPath); // constructor
	void use() const; // use the shader

	void setBool(const char* name, bool value); // set a boolean uniform
	void setInt(const char* name, int value); // set an integer uniform
	void setFloat(const char* name, float value); // set a float uniform
	void setVec2(const char* name, glm::vec2 value); // set a vec2 uniform
	void setVec3(const char* name, glm::vec3 value); // set a vec3 uniform
	void setVec4(const char* name, glm::vec4 value); // set a vec4 uniform
	void setMat4(const char* name, glm::mat4 value); // set a mat4 uniform
private:
	std::map<const char*, int> uniformLocations; // a map of uniform locations
	int getUniformLocation(const char* name); // get the location of a uniform
	std::map<const char*, bool> boolCache;
	std::map<const char*, int> intCache;
	std::map<const char*, float> floatCache;
	std::map<const char*, glm::vec2> vec2Cache;
	std::map<const char*, glm::vec3> vec3Cache;
	std::map<const char*, glm::vec4> vec4Cache;
	std::map<const char*, glm::mat4> mat4Cache;
};

