#pragma once
#include <glm\glm.hpp>
#include "Colliders.h"
#include "Rigidbody.h"
#include <vector>

using glm::vec3;

class gameObject
{

protected:

	//Rigidbody* Rigidbody;
	//std::vector<Colliders::Collider*> colliders;


public:
	vec3 position;

	virtual void Update() {};

};
