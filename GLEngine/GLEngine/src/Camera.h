#pragma once
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "BoundingVolumes.h"

using glm::vec2;
using glm::vec3;
using glm::vec4;

class Camera
{
protected:
	glm::mat4 worldTransform;
	glm::mat4 viewTransform;
	glm::mat4 projectionTransform;
	glm::mat4 projectionViewTransform;

	Frustrum frustrum;

	void UpdateProjectionViewTransform();

public:

	//Camera();
	//virtual ~Camera();	
	virtual void initialize() {};
	virtual void update() {};


	void setPerspective(float FOV, float aspectRatio, float nearPlane = 0.1f, float farPlane = 1000.f);
	void setOrthographic(float left, float right, float bottom, float top);
	void LookAt(vec3 position, vec3 target, vec3 worldUp = vec3(0, 1, 0));
	void setPosition(vec3 position);
	vec3 getPosition() const;
	void setWorldTransform(const glm::mat4& transform);
	const glm::mat4& getWorldTransform() const;
	const glm::mat4& getViewTransform() const;
	const glm::mat4& getProjectionTransform() const;
	const glm::mat4& getProjectionViewTransform() const;

	Frustrum& getFrustrum();
};