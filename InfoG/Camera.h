#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	void recalculateView();
	virtual void recalculateProjection() = 0;

public :
	
	glm::mat4 calculateMVP(glm::mat4 model);

	ProjectionType getProjectionType() { return projectionType; };

	void setPosition(glm::vec3 position);
	void setLookAt(glm::vec3 lookAt);
	void setFCP(float fcp);
	void setNCP(float ncp);

};