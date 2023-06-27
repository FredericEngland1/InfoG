#pragma once

#include "Window.h"
#include "RObjectManager.h"

class WCameraProperties : public Window {
private:

	Camera& camera;

public:

	WCameraProperties(Camera& camera);
	WCameraProperties() = default;

	void render() override;

};