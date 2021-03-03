#pragma once

#include <GL/gl3w.h>

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

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
	Mesh mesh;
	Texture texture;
	Shader shader;

	unsigned int MVPMatrixID;

	void initialyzeMesh();
	void initialyzeTexture();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 modelMatrix;

	void updateModelMatrix();

	void setID(unsigned int ID) { RObject::ID = ID; };

	friend class RObjectManager;

public :

	RObject(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices, unsigned int meshType, 
		std::string name = "Default", glm::vec3 position = {0,0,0}, std::string VSPath = "PBS.shader", std::string FSPath = "GreyFShader.shader");
	RObject(Mesh m, std::string name = "Default", glm::vec3 position = { 0,0,0 }, std::string VSPath = "PBS.shader", std::string FSPath = "GreyFShader.shader");

	//RObject() = default;

	void initialyze(std::string VSPath, std::string FSPath);
	void render();

	glm::vec3 getPosition() { return position; };
	glm::vec3 setPosition(glm::vec3 position) { RObject::position = position; updateModelMatrix(); };
	glm::vec3 getRotation() { return rotation; };
	glm::vec3 setRotation(glm::vec3 rotation) { RObject::rotation = rotation; updateModelMatrix();};
	glm::vec3 getScale() { return scale; };
	glm::vec3 setScale(glm::vec3 scale) { RObject::scale = scale; updateModelMatrix();};

	glm::mat4 getModelMatrix() { return modelMatrix; };

	std::string getName() { return name; };

	unsigned int getMVPMatrixID() { return MVPMatrixID; };

	unsigned int getID() { return ID; };

	void setShader(std::string VSPath, std::string FSPath);
	void setShader(Shader shader);

	// TODO add a way to change the current shader

};