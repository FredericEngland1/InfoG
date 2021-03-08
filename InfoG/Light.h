#pragma once

enum LightType {
	spot,
	directional,
	point
};

struct Light {

	LightType lightType;

	Light(LightType lightType) : lightType(lightType) {};

	unsigned int getID() { return ID; };
	void setID(unsigned int cID) { ID = cID; };

private :
	unsigned int ID;
};