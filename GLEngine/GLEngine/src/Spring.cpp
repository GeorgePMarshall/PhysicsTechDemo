#include "Spring.h"
#include "Rigidbody.h"
#include "Gizmos.h"
#include <glm\glm.hpp>

Spring::Spring(Rigidbody* object1, Rigidbody* object2, float springConstant, float damping)
{
	this->object1 = object1;
	this->object2 = object2;

	this->springConstant = springConstant;
	this->damping = damping;

	this->restLength = glm::length(*(object1->position) - *(object2->position));


}


void Spring::Update()
{
	glm::vec3 difference = *(object1->position) - *(object2->position);
	glm::vec3 relativeVelocity = object1->velocity - object2->velocity;
	float springLength = glm::length(difference);
	float force = -springConstant * (springLength - restLength);
	glm::vec3 forceVector = (glm::normalize(difference) * force) - (damping * relativeVelocity);

	object1->AddForce(forceVector);
	object2->AddForce(-forceVector);
}

void Spring::Draw()
{
	Gizmos::addLine(*(object1->position), *(object2->position), glm::vec4(1));
}

