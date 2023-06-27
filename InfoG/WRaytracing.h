#pragma once

#include "Window.h"
#include "Camera.h"

#include <opencv2/opencv.hpp>
#include <random>

#include "RObject.h"
#include "RObjectManager.h"

class WRaytracing : public Window {

private :

	enum MatType {
		refraction,
		specular
	};

	struct Sphere {
		MatType type;

		float rayon;
		glm::vec3 position;

		Sphere(MatType type = refraction, float rayon = 5.0f, glm::vec3 position = { 0.0f, 0.0f, 0.0f }) : type(type), rayon(rayon), position(position) {}
	};

	Camera& camera;
	std::string name;

	cv::Mat img;

	bool intersectRayBox(glm::vec3 rOrigin, glm::vec3 rVector, RObject rObject);

	void updateObjectBounds();

	void threadMethod(int subsamplingSize, int subsamplingSqrt, glm::vec3 axis_x, glm::vec3 axis_y, glm::vec3 axis_z);
	glm::vec3 trace(glm::vec3 rOrigin, glm::vec3 rVector, unsigned int depth);
	glm::vec3 trace2(glm::vec3 rOrigin, glm::vec3 rVector, unsigned int depth);
	glm::vec4 intersect(glm::vec3 rOrigin, glm::vec3 rVector, glm::vec3 triangleVector1, glm::vec3 triangleVector2, glm::vec3 triangleVector3);

	float intersectSphere(glm::vec3 rOrigin, glm::vec3 rVector, Sphere sphere);

	const float EPSILON = 0.0000001f;

	unsigned int textureID;

	bool firstUpdate;

	void createAndCompileShader();

	unsigned int shaderProgram;

public :

	WRaytracing(Camera& camera, std::string name = "Raytracing View", unsigned int width = 240, unsigned int height = 135, unsigned int rayPP = 1, unsigned int subsamplingSize = 2); // TODO change rayPP to make sense

	void render() override;
	void renderImage();

	void updateObjectMeshImage();

	// TODO should change this to setters and getters

	unsigned int width;
	unsigned int height;
	unsigned int rayPP;
	unsigned int subsamplingSize;
};