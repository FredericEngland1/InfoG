#include "Camera.h"

void Camera::recalculateView()
{
	view = glm::lookAt(
		position,
		lookAt,
		glm::vec3(0, 1, 0) // Up
	);
}

void Camera::recalculateProjection()
{
}

glm::mat4 Camera::calculateMV(glm::mat4 model)
{
	MV = view * model;
	return MV;
}

glm::mat4 Camera::calculateMVP()
{
	return projection * MV;
}

glm::mat4 Camera::calculateMVP(glm::mat4 model)
{
	return projection * view * model;
}

glm::mat4 Camera::calculateVP()
{
	return projection * view;
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
