#pragma once
#include "Mesh.h"
#include <vector>

class Geometry : Mesh
{
	GLuint vao, vbo, ebo;
	std::vector<vec3> verticies;

public:


	void GenSphere();
	void GenCube();

	void Draw(Camera& camera, ShaderProgram& shader) override;

};
