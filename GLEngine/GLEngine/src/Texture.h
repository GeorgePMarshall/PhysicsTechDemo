#pragma once
#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <stb\stb_image.h>

class Texture
{
	GLuint textureID;

	GLint width, height;

	GLint imageFormat;


public:
	Texture() {};
	Texture(const char* filePath);
	~Texture();

	void Bind();
	void LoadTexture(const char* filePath);
	void LoadTexture(GLfloat* textureData, GLuint width, GLuint height, GLenum internalFormat = GL_R32F, GLenum format = GL_RED);

};