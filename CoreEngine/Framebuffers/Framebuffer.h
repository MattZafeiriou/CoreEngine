#pragma once
#include <glm/glm.hpp>

class Framebuffer
{
public:
	Framebuffer();
	Framebuffer(int width, int height);
	~Framebuffer();
	void Bind();
	void Unbind();
	void Clear();
	void AttachTexture();
	void AttachRenderbuffer();
	void CheckStatus();
	void Delete();
	void Render();
	void Resize(int width, int height);
	void SetClearColor(float r, float g, float b, float a);
	void SetClearColor(float r, float g, float b);
	void SetClearColor(glm::vec4 color);
	void SetClearColor(glm::vec3 color);
	void SetDepthTest(bool value);
	unsigned int GetID();
	unsigned int GetTexture();
	unsigned int GetRenderbuffer();
private:
	unsigned int ID;
	unsigned int texture;
	unsigned int renderbuffer;
	unsigned int width;
	unsigned int height;
	bool depthTest = true;
	glm::vec4 clearColor = glm::vec4(1.0f);
	void Create();
	void CreateTexture();
	void CreateRenderbuffer();
};

