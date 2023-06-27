#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MainWConsole.h"

enum ProjectionType {
	perspective,
	orthographic
};

class Camera {
protected :

	ProjectionType projectionType;

	float ncp;
	float fcp;

	glm::vec3 position;
	glm::vec3 lookAt;

	glm::mat4 view;
	glm::mat4 projection;

	glm::mat4 MV;

	void recalculateView();
	virtual void recalculateProjection();

	unsigned int ID;

	float gamma = 0;

public :
	
	glm::mat4 calculateMV(glm::mat4 model);
	glm::mat4 calculateMVP(); // Will use last MV matrix
	glm::mat4 calculateMVP(glm::mat4 model);

	glm::mat4 calculateVP();

	ProjectionType getProjectionType() { return projectionType; };

	void setPosition(glm::vec3 position);
	void setLookAt(glm::vec3 lookAt);
	void setFCP(float fcp);
	void setNCP(float ncp);

	glm::vec3 getPosition() { return position; };
	glm::vec3 getLookAt() { return lookAt; };

	unsigned int getID() { return ID; };
	void setID(unsigned int cID) { ID = cID; };

	float getFCP() { return fcp; };
	float getNCP() { return ncp; };

	float getGamma() { return gamma; };
	void setGamma(float newGamma) { gamma = newGamma; };

};