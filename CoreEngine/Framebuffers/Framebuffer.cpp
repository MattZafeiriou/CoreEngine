#include "Framebuffer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

Framebuffer::Framebuffer()
{
	width = 800;
	height = 600;
	Create();
}

Framebuffer::Framebuffer(int width, int height)
{
	this->width = width;
	this->height = height;
	Create();
}

Framebuffer::~Framebuffer()
{
	Delete();
}

void Framebuffer::Create()
{
	glGenFramebuffers(1, &ID);
	Bind();

	CreateTexture();
	CreateRenderbuffer();

	CheckStatus();
}

void Framebuffer::CreateTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::CreateRenderbuffer()
{
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::AttachTexture()
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
}

void Framebuffer::AttachRenderbuffer()
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
}

void Framebuffer::Render()
{
	Bind();
	Clear();
}

void Framebuffer::Resize(int width, int height)
{
	this->width = width;
	this->height = height;
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &renderbuffer);
	CreateTexture();
	CreateRenderbuffer();
	CheckStatus();
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	if (depthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::CheckStatus()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
}

void Framebuffer::Clear()
{
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | (depthTest ? GL_DEPTH_BUFFER_BIT : 0));
}

void Framebuffer::Delete()
{
	glDeleteFramebuffers(1, &ID);
}

unsigned int Framebuffer::GetID()
{
	return ID;
}

void Framebuffer::SetClearColor(float r, float g, float b, float a)
{
	clearColor = glm::vec4(r, g, b, a);
}

void Framebuffer::SetClearColor(float r, float g, float b)
{
	clearColor = glm::vec4(r, g, b, 1.0f);
}

void Framebuffer::SetClearColor(glm::vec4 color)
{
	clearColor = color;
}

void Framebuffer::SetClearColor(glm::vec3 color)
{
	clearColor = glm::vec4(color, 1.0f);
}

void Framebuffer::SetDepthTest(bool value)
{
	depthTest = value;
}

unsigned int Framebuffer::GetTexture()
{
	return texture;
}

unsigned int Framebuffer::GetRenderbuffer()
{
	return renderbuffer;
}