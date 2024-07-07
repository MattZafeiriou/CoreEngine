#pragma once
class Shader
{
public:
	unsigned int ID; // the shader ID

	Shader(const char* vertexPath, const char* fragmentPath); // constructor
	void use(); // use the shader

	void setBool(const char* name, bool value) const; // set a boolean uniform
	void setInt(const char* name, int value) const; // set an integer uniform
	void setFloat(const char* name, float value) const; // set a float uniform
};

