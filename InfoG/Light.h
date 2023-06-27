#pragma once

enum LightType {
	spot,
	directional,
	point
};

class Light {
public :

	LightType lightType;

	Light(LightType lightType);

	unsigned int getID() { return ID; };
	void setID(unsigned int cID) { ID = cID; };

private:
	unsigned int ID;
};