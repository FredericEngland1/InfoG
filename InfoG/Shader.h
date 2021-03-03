#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>

class Shader {
private:

	// Verify and logs any error from the compilation of the shader passed in argument
	void compileErrorCheck(unsigned int shader);
	// Create, compiles and attach a type of shader to the program passed in the arguments
	unsigned int compileShader(const std::string& shaderSource, unsigned int shaderType, unsigned int program);
	int createShader(const std::string& vShaderS, const std::string& fShaderS);

	unsigned int program;

public:

	Shader(const std::string& vShader, const std::string& fShader);
	Shader() = default;

	void use();
	void destroy();
	unsigned int getProgramID() { return program; };
};