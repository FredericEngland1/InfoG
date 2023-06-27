#pragma once

#include <GL/gl3w.h>

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"

#include "ShaderManager.h"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// TODO add error checking for all the opengl initialyzation, altough there is already some in the main process; But add some for the console.

class RObject {
private :

	std::string name;
	unsigned int ID;

	unsigned int vao;
	Material material;

	//Texture texture;
	Shader shader;

	unsigned int MVMatrixID;
	unsigned int MVPMatrixID;
	unsigned int normalMatrixID;

	void initialyzeMesh();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 modelMatrix;

	void updateModelMatrix();

	void setID(unsigned int ID) { RObject::ID = ID; };

	friend class RObjectManager;

	float degreeRotation;

	std::string VSPath;
	std::string FSPath;
	std::string GSPath;

public :

	RObject(std::vector<glm::vec3> vertices, std::vector<glm::uvec3> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uv, Material material,
		std::string name = "Default", glm::vec3 position = {0,0,0}, std::string VSPath = "PBS.shader", std::string FSPath = "GreyFShader.shader");
	RObject(Mesh m, std::string name = "Default", glm::vec3 position = { 0,0,0 }, std::string VSPath = "PBS.shader", std::string FSPath = "GreyFShader.shader");

	RObject() = default;

	void initialyze(std::string VSPath, std::string FSPath);
	void render();

	void useShader();

	glm::vec3 getPosition() { return position; };
	void setPosition(glm::vec3 position) { RObject::position = position; updateModelMatrix(); };
	glm::vec3 getRotation() { return rotation; };
	float getDegreeRotation() { return degreeRotation; };
	void setDegreeRotation(float cDegreeRotation) { degreeRotation = cDegreeRotation; updateModelMatrix(); };
	void setRotation(glm::vec3 rotation) { RObject::rotation = rotation; updateModelMatrix();};
	glm::vec3 getScale() { return scale; };
	void setScale(glm::vec3 scale) { RObject::scale = scale; updateModelMatrix();};

	glm::mat4 getModelMatrix() { return modelMatrix; };

	std::string getName() { return name; };
	void setName(std::string iName) { name = iName; };

	unsigned int getMVMatrixID() { return MVMatrixID; };
	unsigned int getMVPMatrixID() { return MVPMatrixID; };
	unsigned int getNormalMatrixID() { return normalMatrixID; };

	unsigned int getID() { return ID; };

	void setShader(std::string VSPath, std::string FSPath, std::string GSPath = "");
	void setShader(Shader shader);

	unsigned int getUniformLocation(std::string location);

	Material getMaterial() { return material; };
	Material& getMaterialRef() { return material; };

	std::string getVSPath() { return VSPath; };
	std::string getFSPath() { return FSPath; };
	std::string getGSPath() { return GSPath; };

	Mesh mesh;
	Mesh getMesh() { return mesh; };

	cv::Mat meshImage;
	unsigned int meshImageHeight;
	unsigned int meshImageWidth;

	unsigned int meshImageIDIn;
	unsigned int meshImageIDOut;

	// TODO add a way to change the current shader
};