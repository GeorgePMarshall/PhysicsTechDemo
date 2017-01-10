#pragma once
#include "Texture.h"
#include "ShaderProgram.h"

class SpecularMaterial
{
	Texture* diffuseTexture;
	Texture* normalMap;
	Texture* specularMap;

public:
	
	void SetDiffuse(const char* path);
	void SetNormal(const char* path);
	void SetSpecular(const char* path);

	void Bind(ShaderProgram* shader);


};