#include "Cube.h"

void Cube::Initialize()
{
	rigidbody.Initialize();
	rigidbody.setPosition(&position);
	rigidbody.AddAABBCollider(vec3(0), vec3(1));
}

void Cube::Update()
{
}

void Cube::Draw()
{
	Gizmos::addAABBFilled(position, vec3(1), vec4(0.5f));
}