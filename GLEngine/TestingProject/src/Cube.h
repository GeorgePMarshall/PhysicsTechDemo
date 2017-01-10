#pragma once
#include <GameObject.h>
#include <Gizmos.h>
#include <Rigidbody.h>
#include <PhysicsManager.h>

using glm::vec4;


class Cube : public gameObject
{

public:
	Rigidbody rigidbody;
	void Initialize();
	void Update();
	void Draw();


};