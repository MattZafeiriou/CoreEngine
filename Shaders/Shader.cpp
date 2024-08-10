#include "Shader.h"
#include "../Utils/EnvironmentVariablesUtils.cpp"
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <fstream>

/*
 * Code by https://learnopengl.com/Getting-started/Shaders
*/
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open((std::string(getEnvironmentVariable("CORE_ASSETS_PATH")) + std::string("Shaders/VertexShaders/") + std::string(vertexPath)).c_str());
        fShaderFile.open((std::string(getEnvironmentVariable("CORE_ASSETS_PATH")) + std::string("Shaders/FragmentShaders/") + std::string(vertexPath)).c_str());
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() const
{
	glUseProgram(ID);
}

void Shader::setBool(const char* name, bool value)
{
    if (boolCache.find(name) != boolCache.end() && boolCache[name] == value)
        return;
    boolCache[name] = value;

    int location = glGetUniformLocation(ID, name);
	glUniform1i(location, (int)value);
}

void Shader::setInt(const char* name, int value)
{
    if (intCache.find(name) != intCache.end() && intCache[name] == value)
        return;
    intCache[name] = value;

    int location = glGetUniformLocation(ID, name);
	glUniform1i(location, value);
}

void Shader::setFloat(const char* name, float value)
{
    if (floatCache.find(name) != floatCache.end() && floatCache[name] == value)
		return;
    floatCache[name] = value;

    int location = glGetUniformLocation(ID, name);
    glUniform1f(location, value);
}

void Shader::setVec2(const char* name, glm::vec2 value)
{
    if (vec2Cache.find(name) != vec2Cache.end() && vec2Cache[name] == value)
        return;
    vec2Cache[name] = value;

    int location = glGetUniformLocation(ID, name);
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void Shader::setVec3(const char* name, glm::vec3 value)
{
    if (vec3Cache.find(name) != vec3Cache.end() && vec3Cache[name] == value)
        return;
    vec3Cache[name] = value;

    int location = glGetUniformLocation(ID, name);
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setVec4(const char* name, glm::vec4 value)
{
    if (vec4Cache.find(name) != vec4Cache.end() && vec4Cache[name] == value)
        return;
    vec4Cache[name] = value;

    int location = glGetUniformLocation(ID, name);
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::setMat4(const char* name, glm::mat4 value)
{
    if (mat4Cache.find(name) != mat4Cache.end() && mat4Cache[name] == value)
		return;
    mat4Cache[name] = value;

    int location = glGetUniformLocation(ID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

int Shader::getUniformLocation(const char* name)
{
	if (uniformLocations.find(name) == uniformLocations.end())
	{
		uniformLocations[name] = glGetUniformLocation(ID, name);
	}
	return uniformLocations[name];
}