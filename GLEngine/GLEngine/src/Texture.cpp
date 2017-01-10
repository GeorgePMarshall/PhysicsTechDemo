#include "Texture.h"

Texture::Texture(const char* filePath)
{
	LoadTexture(filePath);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureID);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::LoadTexture(const char* filePath)
{
	GLubyte* texData = stbi_load(filePath, &width, &height, &imageFormat, STBI_default);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, imageFormat == STBI_rgb_alpha ? GL_RGBA : GL_RGB, width, height, 0, imageFormat == STBI_rgb_alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	stbi_image_free(texData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::LoadTexture(GLfloat* textureData, GLuint width, GLuint height, GLenum internalFormat, GLenum format)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, textureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}