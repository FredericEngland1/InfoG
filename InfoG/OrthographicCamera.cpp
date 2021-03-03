#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, glm::vec3 position, float ncp, float fcp) : left(left), right(right), bottom(bottom), top(top)
{
	Camera::position = position;
	Camera::ncp = ncp;
	Camera::fcp = fcp;

	projectionType = ProjectionType::orthographic;

	lookAt = { 0,0,0 }; // TODO set to look in front of position (position + {0,0,10})

	recalculateView();
	recalculateProjection();
}

void OrthographicCamera::setSize(float left, float right, float bottom, float top)
{
	OrthographicCamera::left = left;
	OrthographicCamera::right = right;
	OrthographicCamera::bottom = bottom;
	OrthographicCamera::top = top;

	recalculateProjection();
}

void OrthographicCamera::recalculateProjection()
{
	projection = glm::ortho(
		left,
		right,
		bottom,
		top,
		ncp,
		fcp
	);
}