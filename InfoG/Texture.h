#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <GL/gl3w.h>

#include "Image.h"

class Texture {
private :

	Image image;
	std::string path;

	unsigned int textureID;

	void bind();

public :

	Texture() = default;
	Texture(std::string path);

	unsigned int getTextureID() { return textureID; };
	Image getImage() { return image; };
	void randomImage();
};