#pragma once
#include <FbxLoader\FBXFile.h>
#include <iostream>
#include "ShaderProgram.h"
#include "BoundingVolumes.h"
#include "Camera.h"

class Mesh
{
	FBXFile* meshData;

	BoundingSphere* boundingSpheres;
	

public:
	glm::mat4 transform;

	Mesh();
	//~Mesh();

	void LoadMesh(const char* filePath);
	void CreateBuffers();
	void RemoveBuffers();


	void translate(vec3 position);
	void scale(float scalar);

	virtual void Draw(Camera& camera, ShaderProgram& shader);


};

