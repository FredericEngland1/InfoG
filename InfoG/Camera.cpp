#include "Camera.h"

void Camera::recalculateView()
{
	view = glm::lookAt(
		position,
		lookAt,
		glm::vec3(0, 1, 0) // Up
	);
}

glm::mat4 Camera::calculateMVP(glm::mat4 model)
{
	return projection * view * model;
}

void Camera::setPosition(glm::vec3 position)
{
	Camera::position = position;
	recalculateView();
}

void Camera::setLookAt(glm::vec3 lookAt)
{
	Camera::lookAt = lookAt;
	recalculateView();
}

void Camera::setFCP(float fcp)
{
	Camera::fcp = fcp;
	recalculateProjection();
}

void Camera::setNCP(float ncp)
{
	Camera::ncp = ncp;
	recalculateProjection();
}

// TODO add a way to set a camera from perspective to orthographic
