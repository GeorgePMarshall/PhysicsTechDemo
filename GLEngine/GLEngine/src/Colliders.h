#pragma once
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>
using glm::vec3;


namespace Colliders
{
	struct Collider
	{};


	struct Sphere : public Collider
	{
		vec3 center;
		GLfloat radius;

		Sphere(vec3 center, GLfloat radius) { this->center = center; this->radius = radius; };
	};
	
	
	struct AABB : public Collider
	{
		vec3 center;
		vec3 extents;

		AABB(vec3 center, vec3 extents) { this->center = center, this->extents = extents; };
	};

	
	struct Plane : public Collider
	{
		vec3 normal;
		GLfloat distance;

		Plane(vec3 normal, GLfloat distance) { this->normal = normal; this->distance = distance; };
	};
	

	
	bool Intersects(Sphere& sphere, Plane& plane);
	bool Intersects(AABB& aabb, AABB& aabb2);

	vec3 Collision(Sphere& sphere, Plane& plane);

}


