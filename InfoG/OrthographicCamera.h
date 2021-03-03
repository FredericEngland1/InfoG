#pragma once

#include "Camera.h"

class OrthographicCamera : public Camera {
private :

	float left;
	float right;
	float bottom;
	float top;

	void recalculateProjection() override;

public :

	OrthographicCamera(float left = 0, float right = 0, float bottom = 0, float top = 0, glm::vec3 position = { 0,0,0 }, float ncp = 0.1f, float fcp = 100.0f);

	void setSize(float left, float right, float bottom, float top);

};