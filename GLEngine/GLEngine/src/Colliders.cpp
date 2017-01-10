#include "Colliders.h"


bool Colliders::Intersects(Sphere& sphere, Plane& plane)
{
	float sphereToPlane = glm::dot(sphere.center, plane.normal) - plane.distance;
	if (sphereToPlane < 0 + sphere.radius)
	{
		return true;
	}
	return false;

}

bool Colliders::Intersects(AABB& aabb, AABB& aabb2)
{
	vec3 min, max, min2, max2;

	min = aabb.center - aabb.extents;
	max = aabb.center + aabb.extents;
	min2 = aabb2.center - aabb2.extents;
	max2 = aabb2.center + aabb2.extents;


	if (min.x > max2.x)
		return false;

	if (min.y > max2.y)
		return false;

	if (min.z > max2.z)
		return false;


	if (min2.x > max.x)
		return false;

	if (min2.y > max.y)
		return false;

	if (min2.z > max.z)
		return false;


	return true;


}

vec3 Colliders::Collision(Sphere& sphere, Plane& plane)
{
	float sphereToPlane = glm::dot(sphere.center, plane.normal) - plane.distance;
	if (sphereToPlane < 0 + sphere.radius)
	{
		return vec3(0);
	}
	return vec3(0);

}
