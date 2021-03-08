#pragma once

#include "Window.h"
#include "RObjectManager.h"

class WProperties : public Window {
private :

	std::vector<unsigned int> previousSelection;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	float degreeRotation;

	bool openHisto;

public :

	void render() override;

};