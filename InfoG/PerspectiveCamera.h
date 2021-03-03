#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera {
private:

	float FOV;
	float aspectRatio;

	void recalculateProjection() override;

public:

	PerspectiveCamera(float aspectRatio = 16/9, float FOV = 90, glm::vec3 position = { 0,0,0 }, float ncp = 0.1f, float fcp = 100.0f);

	void setFOV(float FOV);
	void setAspectRatio(float aspectRatio);

};