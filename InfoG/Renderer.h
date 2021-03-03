#pragma once

#include "Camera.h"
#include "PerspectiveCamera.h"
#include "RObject.h"
#include "RObjectManager.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Renderer {
private :

	Camera* camera = nullptr; // shared pointer ?

public :

	Renderer(Camera& camera);

	void render();

};