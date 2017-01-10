#include "SpecularMaterial.h"


void SpecularMaterial::SetDiffuse(const char* path)
{
	diffuseTexture = new Texture(path);
}

void SpecularMaterial::SetNormal(const char* path)
{
	normalMap = new Texture(path);
}

void SpecularMaterial::SetSpecular(const char* path)
{
	specularMap = new Texture(path);
}

void SpecularMaterial::Bind(ShaderProgram* shader)
{
	if (diffuseTexture)
	{
		shader->setInt("material.diffuse", 0);
		glActiveTexture(GL_TEXTURE0);
		diffuseTexture->Bind();
	}

	if (normalMap)
	{
		shader->setInt("material.normal", 1);
		glActiveTexture(GL_TEXTURE1);
		normalMap->Bind();
	}

	if (specularMap)
	{
		shader->setInt("material.specular", 2);
		glActiveTexture(GL_TEXTURE2);
		specularMap->Bind();
	}
}