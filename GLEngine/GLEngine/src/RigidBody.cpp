#include "Rigidbody.h"
#include <limits>
#include <iostream>
#include <math.h>

void Rigidbody::Initialize(GLfloat mass, GLfloat elasticity, vec3 initialVelocity, vec3 initialAcceleration, vec3 initialForce)
{
	this->mass = mass;
	this->elasticity = elasticity;
	this->velocity = velocity;
	this->acceleration = acceleration;
	this->force = force;

}

void Rigidbody::AddForce(vec3 force)
{
	this->force += force;
}

void Rigidbody::AddSphereCollider(vec3& relativePosition, GLfloat radius)
{
	sphereColiders.push_back(new Colliders::Sphere(relativePosition, radius));
}

void Rigidbody::AddPlaneCollider(vec3& normal, GLfloat distance)
{
	planeColiders.push_back(new Colliders::Plane(normal, distance));
}

void Rigidbody::AddAABBCollider(vec3& center, vec3& extents)
{
	aabbColiders.push_back(new Colliders::AABB(center, extents));
}

void Rigidbody::Update()
{
	if (!isKinematic)
	{
		velocity *= 0.9999999;
		acceleration = glm::vec3(0, -9.8f, 0);
		acceleration += force / mass;
		velocity += acceleration * gTime::physicsTimeStep();
		*position += velocity * gTime::physicsTimeStep();



		force = vec3(0);
	}
	else
	{
		velocity = vec3(0);
	}

	for each (Colliders::Sphere* collider in sphereColiders)
	{
		//TODO- make good code VVVVVVVVVV
		collider->center = *position;
	}
	for each (Colliders::AABB* collider in aabbColiders)
	{
		//TODO- make good code VVVVVVVVVV
		collider->center = *position;
	}
	
}

void Rigidbody::SphereSphereColision(Rigidbody& other, Colliders::Sphere* collider, Colliders::Sphere* otherCollider )
{
	glm::vec3 difference = collider->center - otherCollider->center;
	GLfloat distance = glm::length(difference);
	GLfloat intersection = collider->radius + otherCollider->radius - distance;
	if (intersection > 0)
	{
		glm::vec3 collisionNormal = glm::normalize(difference);
		glm::vec3 relativeVelocity = velocity - other.velocity;
		float massRatio = (1.0f / mass + 1.0f / other.mass);

		float top = glm::dot((-(1.0f + elasticity) * relativeVelocity), collisionNormal);
		float bottom;
		if (isKinematic || other.isKinematic)
			bottom = glm::dot(collisionNormal, collisionNormal);
		else
			bottom = glm::dot(collisionNormal, collisionNormal * massRatio);

		float impulse = top / bottom;

		velocity += (impulse / mass) * collisionNormal;
		other.velocity -= (impulse / other.mass) * collisionNormal;


		glm::vec3 seperationVector = collisionNormal * intersection * 0.5f;

		if (!isKinematic)
			(*position) += seperationVector;

		if (!other.isKinematic)
			*(other.position) -= seperationVector;
	}
}

void Rigidbody::SpherePlaneColision(Rigidbody& other, Colliders::Sphere* collider, Colliders::Plane* otherCollider)
{
	GLfloat sphereToPlane = glm::dot(collider->center, otherCollider->normal) - otherCollider->distance;

	glm::vec3 collisionNormal = otherCollider->normal;

	if (sphereToPlane < 0)
	{
		collisionNormal *= -1;
		sphereToPlane *= -1;
	}
	GLfloat intersection = collider->radius - sphereToPlane;

	if (intersection > 0)
	{
		glm::vec3 planeNormal = otherCollider->normal;

		if (sphereToPlane < 0)
			planeNormal *= -1;

		glm::vec3 relativeVelocity = velocity - other.velocity;
		float massRatio = (1.0f / mass + 1.0f / other.mass);

		float top = glm::dot((-(1.0f + elasticity)*relativeVelocity), collisionNormal);
		float bottom;
		if (isKinematic || other.isKinematic)
			bottom = glm::dot(collisionNormal, collisionNormal);
		else
			bottom = glm::dot(collisionNormal, collisionNormal * massRatio);


		float impulse = top / bottom;


		velocity = velocity + (impulse / mass) * collisionNormal;
		*position += collisionNormal * intersection * 0.5f;
	}

}

void Rigidbody::PlaneSphereColision(Rigidbody& other, Colliders::Plane* collider, Colliders::Sphere* otherCollider)
{
	GLfloat sphereToPlane = glm::dot(otherCollider->center, collider->normal) - collider->distance;

	glm::vec3 collisionNormal = collider->normal;

	if (sphereToPlane < 0)
	{
		collisionNormal *= -1;
		sphereToPlane *= -1;
	}
	GLfloat intersection = otherCollider->radius - sphereToPlane;

	if (intersection > 0)
	{
		glm::vec3 planeNormal = collider->normal;

		if (sphereToPlane < 0)
			planeNormal *= -1;


		glm::vec3 relativeVelocity = other.velocity - velocity;
		float massRatio = (1.0f / mass + 1.0f / other.mass);

		float top = glm::dot((-(1.0f + elasticity)*relativeVelocity), collisionNormal);
		float bottom;
		if (isKinematic || other.isKinematic)
			bottom = glm::dot(collisionNormal, collisionNormal);
		else
			bottom = glm::dot(collisionNormal, collisionNormal * massRatio);

		float impulse = top / bottom;


		other.velocity = other.velocity + (impulse / other.mass) * collisionNormal;
		*(other.position) += collisionNormal * intersection * 0.5f;
	}
}

void Rigidbody::AABBAABBColision(Rigidbody& other, Colliders::AABB* collider, Colliders::AABB* otherCollider)
{
	vec3 min, max, min2, max2;

	min = collider->center - collider->extents;
	max = collider->center + collider->extents;
	min2 = otherCollider->center - otherCollider->extents;
	max2 = otherCollider->center + otherCollider->extents;

	vec3 minOffset = max - min2;
	vec3 maxOffset = min - max2;

	if (isnan(minOffset.x) || isnan(minOffset.y) || isnan(minOffset.z) || isnan(minOffset.x) || isnan(minOffset.y) || isnan(minOffset.z))
		return;

	if (!(minOffset.x < 0 || minOffset.y < 0 || minOffset.z < 0 || maxOffset.x > 0 || maxOffset.y > 0 || maxOffset.z > 0))
	{
		vec3 offset;

		offset.x = (abs(minOffset.x) < abs(maxOffset.x)) ? minOffset.x : maxOffset.x;
		offset.y = (abs(minOffset.y) < abs(maxOffset.y)) ? minOffset.y : maxOffset.y;
		offset.z = (abs(minOffset.z) < abs(maxOffset.z)) ? minOffset.z : maxOffset.z;

		offset.x = abs(offset.x) < abs(offset.y) ? offset.x : 0.0f;
		offset.y = abs(offset.y) < abs(offset.z) ? offset.y : 0.0f;
		offset.z = abs(offset.z) < abs(offset.x) ? offset.z : 0.0f;

		glm::vec3 collisionNormal = glm::normalize(offset);
		glm::vec3 relativeVelocity = velocity - other.velocity;
		float massRatio = (1.0f / mass + 1.0f / other.mass);

		float top = glm::dot((-(1.0f + elasticity) * relativeVelocity), collisionNormal);
		float bottom;
		if (isKinematic || other.isKinematic)
			bottom = glm::dot(collisionNormal, collisionNormal);
		else
			bottom = glm::dot(collisionNormal, collisionNormal * massRatio);

		float impulse = top / bottom;

		velocity += (impulse / mass) * collisionNormal;
		other.velocity -= (impulse / other.mass) * collisionNormal;


		glm::vec3 seperationVector = collisionNormal * glm::length(offset) * 0.5f;

		if (!isKinematic)
			(*position) -= seperationVector;

		if (!other.isKinematic)
			(*other.position) += seperationVector;



	}

}

void Rigidbody::AABBSphereColision(Rigidbody& other, Colliders::AABB* collider, Colliders::Sphere* otherCollider)
{
	vec3 min, max;

	min = collider->center - collider->extents;
	max = collider->center + collider->extents;

	vec3 offset = otherCollider->center - collider->center;

	if (std::abs(offset.x) > 0)
		offset.x = glm::min(std::abs(offset.x), collider->extents.x) * (std::abs(offset.x) / offset.x);

	if (std::abs(offset.y) > 0)
		offset.y = glm::min(std::abs(offset.y), collider->extents.y) * (std::abs(offset.y) / offset.y);

	if (std::abs(offset.z) > 0)
		offset.z = glm::min(std::abs(offset.z), collider->extents.z) * (std::abs(offset.z) / offset.z);

	vec3 collisionPoint = collider->center + offset;
	offset = collisionPoint - otherCollider->center;

	float intersection = otherCollider->radius - glm::length(offset);
	
	if (intersection > 0)
	{
		glm::vec3 collisionNormal = glm::normalize(-offset);
		glm::vec3 relativeVelocity = velocity - other.velocity;
		float massRatio = (1.0f / mass + 1.0f / other.mass);

		float top = glm::dot((-(1.0f + elasticity / 2) * relativeVelocity), collisionNormal);
		float bottom;
		if (isKinematic || other.isKinematic)
			bottom = glm::dot(collisionNormal, collisionNormal);
		else
			bottom = glm::dot(collisionNormal, collisionNormal * massRatio);

		float impulse = top / bottom;

		velocity += (impulse / mass) * collisionNormal;
		other.velocity -= (impulse / other.mass) * collisionNormal;

		glm::vec3 seperationVector = collisionNormal * intersection * 0.5f;

		if (!isKinematic)
			(*position) -= seperationVector;

		if (!other.isKinematic)
			(*other.position) += seperationVector;
	}

}

void Rigidbody::SphereAABBColision(Rigidbody& other, Colliders::Sphere* collider, Colliders::AABB* otherCollider)
{
	vec3 min, max;

	min = otherCollider->center - otherCollider->extents;
	max = otherCollider->center + otherCollider->extents;

	vec3 offset = collider->center - otherCollider->center;

	if (std::abs(offset.x) > 0)
		offset.x = glm::min(std::abs(offset.x), otherCollider->extents.x) * (std::abs(offset.x) / offset.x);

	if (std::abs(offset.y) > 0)
		offset.y = glm::min(std::abs(offset.y), otherCollider->extents.y) * (std::abs(offset.y) / offset.y);

	if (std::abs(offset.z) > 0)
		offset.z = glm::min(std::abs(offset.z), otherCollider->extents.z) * (std::abs(offset.z) / offset.z);

	vec3 collisionPoint = otherCollider->center + offset;
	offset = collisionPoint - collider->center;

	float intersection = collider->radius - glm::length(offset);
	if (intersection > 0)
	{
		glm::vec3 collisionNormal = glm::normalize(offset);
		glm::vec3 relativeVelocity = velocity - other.velocity;
		float massRatio = (1.0f / mass + 1.0f / other.mass);

		float top = glm::dot((-(1.0f + elasticity / 2) * relativeVelocity), collisionNormal);
		float bottom;
		if (isKinematic || other.isKinematic)
			bottom = glm::dot(collisionNormal, collisionNormal);
		else
			bottom = glm::dot(collisionNormal, collisionNormal * massRatio);

		float impulse = top / bottom;

		velocity += (impulse / mass) * collisionNormal;
		other.velocity -= (impulse / other.mass) * collisionNormal;

		glm::vec3 seperationVector = collisionNormal * intersection * 0.5f;

		if (!isKinematic)
			(*position) -= seperationVector;

		if (!other.isKinematic)
			(*other.position) += seperationVector;
	}

}

void Rigidbody::PlaneAABBColision(Rigidbody& other, Colliders::Plane* collider, Colliders::AABB* otherCollider)
{
	vec3 min, max;

	min = otherCollider->center - otherCollider->extents;
	max = otherCollider->center + otherCollider->extents;

	float minDist = glm::dot(min, collider->normal) - collider->distance;
	float maxDist = glm::dot(max, collider->normal) - collider->distance;

	float intersection = -glm::min(minDist, maxDist);

	if (intersection > 0)
	{
		glm::vec3 collisionNormal = -collider->normal;
		glm::vec3 relativeVelocity = velocity - other.velocity;
		float massRatio = (1.0f / mass + 1.0f / other.mass);

		float top = glm::dot((-(1.0f + elasticity) * relativeVelocity), collisionNormal);
		float bottom;
		if (isKinematic || other.isKinematic)
			bottom = glm::dot(collisionNormal, collisionNormal);
		else
			bottom = glm::dot(collisionNormal, collisionNormal * massRatio);

		float impulse = top / bottom;

		//velocity += (impulse / mass) * collisionNormal;
		other.velocity -= (impulse / other.mass) * collisionNormal;

		glm::vec3 seperationVector = collisionNormal * intersection * 0.5f;

		if (!isKinematic)
			(*position) += seperationVector;

		if (!other.isKinematic)
			(*other.position) -= seperationVector;
	}
}


void Rigidbody::resolveColision(Rigidbody& other)
{
	for (Colliders::Sphere* collider : sphereColiders)
	{
		for (Colliders::Sphere* otherCollider : other.sphereColiders)
		{
			SphereSphereColision(other, collider, otherCollider);
		}
	}

	for (Colliders::Sphere* collider : sphereColiders)
	{
		for (Colliders::Plane* otherCollider : other.planeColiders)
		{
			SpherePlaneColision(other, collider, otherCollider);
		}
	}

	for (Colliders::Plane* collider : planeColiders)
	{
		for (Colliders::Sphere* otherCollider : other.sphereColiders)
		{
			PlaneSphereColision(other, collider, otherCollider);
		}
	}

	for (Colliders::Plane* collider : planeColiders)
	{
		for (Colliders::AABB* otherCollider : other.aabbColiders)
		{
			PlaneAABBColision(other, collider, otherCollider);
		}
	}

	for (Colliders::Sphere* collider : sphereColiders)
	{
		for (Colliders::AABB* otherCollider : other.aabbColiders)
		{
			SphereAABBColision(other, collider, otherCollider);			
		}
	}

	for (Colliders::AABB* collider : aabbColiders)
	{
		for (Colliders::Sphere* otherCollider : other.sphereColiders)
		{
			AABBSphereColision(other, collider, otherCollider);
		}
	}


	for (Colliders::AABB* collider : aabbColiders)
	{
		for (Colliders::AABB* otherCollider : other.aabbColiders)
		{	
			AABBAABBColision(other, collider, otherCollider);
		}
	}


}
