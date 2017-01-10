#include "Camera.h"


//Camera::Camera()
//{
//	worldTransform = glm::mat4();
//	UpdateProjectionViewTransform();
//}
//
//Camera::~Camera()
//{
//
//}





void Camera::UpdateProjectionViewTransform()
{
	viewTransform = glm::inverse(worldTransform);
	projectionViewTransform = projectionTransform * viewTransform;
	frustrum.calculateFrustrum(projectionViewTransform);
}


void Camera::setPerspective(float FOV, float aspectRatio, float nearPlane, float farPlane)
{
	projectionTransform = glm::perspective(FOV, aspectRatio, nearPlane, farPlane);
	UpdateProjectionViewTransform();
}
void Camera::setOrthographic(float left, float right, float bottom, float top)
{
	projectionTransform = glm::ortho(left, right, bottom, top);
	UpdateProjectionViewTransform();
}
void Camera::LookAt(vec3 position, vec3 target, vec3 worldUp)
{
	worldTransform = glm::inverse(glm::lookAt(position, target, worldUp));
	UpdateProjectionViewTransform();
}
void Camera::setPosition(vec3 position)
{
	worldTransform[3] = vec4(position, 1);
	UpdateProjectionViewTransform();
}
 vec3 Camera::getPosition() const
{
	return worldTransform[3].xyz;
}
void Camera::setWorldTransform(const glm::mat4& transform)
{
	worldTransform = transform;
	UpdateProjectionViewTransform();
}


const glm::mat4& Camera::getWorldTransform() const
{
	return worldTransform;
}
const glm::mat4& Camera::getViewTransform() const 
{
	return viewTransform;
}
const glm::mat4& Camera::getProjectionTransform() const
{			   
	return projectionTransform;
}			  
const glm::mat4& Camera::getProjectionViewTransform() const
{
	return projectionViewTransform;
}

Frustrum& Camera::getFrustrum()
{
	return frustrum;
}