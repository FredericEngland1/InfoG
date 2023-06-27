#pragma once

#include "Window.h"
#include "Renderer.h"

#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>
#include <chrono>

#include <cmath>

#include <string>

class WCamera : public Window {
private :

	Renderer renderer;
	Camera& camera;
	std::string name;

	glm::vec4 clearColor;

public :

	WCamera(Camera& camera, std::string name = "Camera View", glm::vec4 clearColor = {0,0,0,1});

	void render() override;
	void setImagesToOutput(unsigned int ITO) { renderer.setImagesToOutput(ITO); };
	unsigned int getImagesToOutput() { return renderer.getImagesToOutput(); };

	glm::vec4 getClearColor() { return clearColor; };
	void setClearColor(glm::vec4 cc) { clearColor = cc; };
};