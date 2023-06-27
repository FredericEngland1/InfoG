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
	unsigned int framebuffer;
	unsigned int texture;
	unsigned int depthrenderbuffer;
	unsigned int imagesToOutput;

	Shader cubemapShader;
	std::vector<Image> skyboxImages;

	unsigned int vaoSkybox;
	unsigned int textureIDSkybox;

public :

	Renderer(Camera& camera);
	void drawSkybox();
	Renderer() = default;

	unsigned int render(unsigned int width, unsigned int height, glm::vec4 clearColor = { 0,0,0,1 });

	unsigned int getFramebuffer() { return framebuffer; };

	void setImagesToOutput(unsigned int ITO) { imagesToOutput = ITO; };
	unsigned int getImagesToOutput() { return imagesToOutput; };

};