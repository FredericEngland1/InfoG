#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


enum TextureType {
	plainColor, // TODO find a better name for just a color
	uv
};

class Texture {
private :

	TextureType type;

	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normal;
	
	glm::vec4 color;

public :

	TextureType getType() { return type; };

};