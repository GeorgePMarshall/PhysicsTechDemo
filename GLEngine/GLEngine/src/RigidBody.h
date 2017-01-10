#pragma once
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "Colliders.h"
#include <vector>
#include "gTime.h"

using glm::vec3;

class Rigidbody
{
	std::vector<Colliders::Sphere*> sphereColiders;
	std::vector<Colliders::Plane*> planeColiders;
	std::vector<Colliders::AABB*> aabbColiders;

	bool isKinematic;

public:
	vec3* position;
	vec3 velocity;
	vec3 acceleration;
	vec3 force;

	GLfloat mass;
	GLfloat elasticity;

	void Initialize(GLfloat mass = 1.f, GLfloat elasticity = 0.5f, vec3 initialVelocity = vec3(0), vec3 initialAcceleration = vec3(0), vec3 initialForce = vec3(0));
	
	inline void setPosition(vec3* position) { this->position = position; };
	inline void setKinematic(bool input) { this->isKinematic = input; };

	void AddSphereCollider(vec3& relativePosition, GLfloat radius);
	void AddPlaneCollider(vec3& normal, GLfloat distance);
	void AddAABBCollider(vec3& relativePosition, vec3& size);
	void resolveColision(Rigidbody& other);
	
	void SphereSphereColision(Rigidbody& other, Colliders::Sphere* collider, Colliders::Sphere* otherCollider);
	
	void SpherePlaneColision(Rigidbody& other, Colliders::Sphere* collider, Colliders::Plane* otherCollider);
	void PlaneSphereColision(Rigidbody& other, Colliders::Plane* collider, Colliders::Sphere* otherCollider);

	void AABBAABBColision(Rigidbody& other, Colliders::AABB* collider, Colliders::AABB* otherCollider);

	void AABBSphereColision(Rigidbody& other, Colliders::AABB* collider, Colliders::Sphere* otherCollider);
	void SphereAABBColision(Rigidbody& other, Colliders::Sphere* collider, Colliders::AABB* otherCollider);

	void AABBPlaneColision(Rigidbody& other, Colliders::Sphere* collider, Colliders::Sphere* otherCollider);
	void PlaneAABBColision(Rigidbody& other, Colliders::Plane* collider, Colliders::AABB* otherCollider);

	void AddForce(vec3 force);
	void AddImpulse(vec3 force);
	void Update();

};
