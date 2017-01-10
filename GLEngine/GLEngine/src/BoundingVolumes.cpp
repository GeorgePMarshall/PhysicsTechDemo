#include "BoundingVolumes.h"
#include "Gizmos.h"

//creates a sphere that will fit the mesh
void BoundingSphere::calculateSphere(FBXMeshNode* mesh)
{
	Gizmos::create();

	glm::vec3 min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::lowest());

	for each (FBXVertex vert in mesh->m_vertices)
	{
		if (vert.position.x < min.x) min.x = vert.position.x;
		if (vert.position.y < min.y) min.y = vert.position.y;
		if (vert.position.x < min.z) min.z = vert.position.z;

		if (vert.position.x > max.x) max.x = vert.position.x;
		if (vert.position.y > max.y) max.y = vert.position.y;
		if (vert.position.x > max.z) max.z = vert.position.z;
	}
	
	this->center = (min + max) * 0.5f;
	this->radius = glm::distance(min, center);

	origCenter = center;
	origRadius = radius;

}

void BoundingSphere::reCalcutalteSphere(glm::mat4& transform)
{
	radius = origRadius * transform[0][0];
	center = vec3(transform * vec4(origCenter, 1));
}

bool BoundingSphere::isColliding(BoundingSphere* other)
{

	if (glm::distance(this->center, other->center) < this->radius + other->radius)
		return true;
	return false;
}

bool BoundingSphere::isColliding(Frustrum& frustrum)
{
	Gizmos::clear();

	for (GLuint i = 0; i < 6; ++i)
	{
		float dot = glm::dot(vec3(frustrum.planes[i]), center) + frustrum.planes[i].w;
		Gizmos::addSphere(center, radius, 32, 32, vec4(1));

		if (dot < -radius)
			return false;
	}
	return true;
}

void Frustrum::calculateFrustrum(glm::mat4& projectionView)
{
	//right
	planes[0] = vec4(projectionView[0][3] - projectionView[0][0],
					 projectionView[1][3] - projectionView[1][0],
					 projectionView[2][3] - projectionView[2][0],
					 projectionView[3][3] - projectionView[3][0]);
	
	//left
	planes[1] =	vec4(projectionView[0][3] + projectionView[0][0],
					 projectionView[1][3] + projectionView[1][0],
					 projectionView[2][3] + projectionView[2][0],
					 projectionView[3][3] + projectionView[3][0]);
	
	//up
	planes[2] = vec4(projectionView[0][3] - projectionView[0][1],
					 projectionView[1][3] - projectionView[1][1],
					 projectionView[2][3] - projectionView[2][1],	
					 projectionView[3][3] - projectionView[3][1]);
	
	//down
	planes[3] = vec4(projectionView[0][3] + projectionView[0][1],
					 projectionView[1][3] + projectionView[1][1],
					 projectionView[2][3] + projectionView[2][1],
					 projectionView[3][3] + projectionView[3][1]);
	
	//far
	planes[4] =	vec4(projectionView[0][3] - projectionView[0][2],
					 projectionView[1][3] - projectionView[1][2],
					 projectionView[2][3] - projectionView[2][2],
					 projectionView[3][3] - projectionView[3][2]);
	
	//near
	planes[5] = vec4(projectionView[0][3] + projectionView[0][2],
					 projectionView[1][3] + projectionView[1][2],
					 projectionView[2][3] + projectionView[2][2],
					 projectionView[3][3] + projectionView[3][2]);
	
	for (GLuint i = 0; i < 6; ++i)
	{
		planes[i] /= glm::length(vec3(planes[i]));
	}


}
