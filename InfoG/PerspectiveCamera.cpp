
#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float aspectRatio, float FOV, glm::vec3 position, float ncp, float fcp) : aspectRatio(aspectRatio), FOV(FOV)
{
	Camera::position = position;
	Camera::ncp = ncp;
	Camera::fcp = fcp;

	projectionType = ProjectionType::perspective;

	lookAt = { 0,0,0 }; // TODO set to look in front of position (position + {0,0,10})

	recalculateView();
	recalculateProjection();
}

void PerspectiveCamera::setFOV(float FOV)
{
	PerspectiveCamera::FOV = FOV;
	recalculateProjection();
}

void PerspectiveCamera::setAspectRatio(float aspectRatio)
{
	PerspectiveCamera::aspectRatio = aspectRatio;
	recalculateProjection();
}

void PerspectiveCamera::recalculateProjection()
{
	projection = glm::perspective(
		glm::radians(FOV),
		aspectRatio,
		ncp,
		fcp
	);
}