#include "Shader.h"

Shader::Shader (const std::string& vShaderPath, const std::string& fShaderPath) {

	std::ifstream vShaderStream(vShaderPath);
	std::stringstream vBuffer;
	vBuffer << vShaderStream.rdbuf();

	std::ifstream gfShaderStream(fShaderPath);
	std::stringstream gfBuffer;
	gfBuffer << gfShaderStream.rdbuf();

	program = createShader(vBuffer.str(), gfBuffer.str());
}

void Shader::destroy()
{
	glDeleteProgram(program);
}

void Shader::use() {
	glUseProgram(program);
}

// Verify and logs any error from the compilation of the shader passed in argument
void Shader::compileErrorCheck(unsigned int shader) {

	int compileResult;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	// If no error finish else log the error
	if (compileResult == GL_FALSE) {

		int logLenght;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLenght);

		std::vector<char> log(logLenght);
		glGetShaderInfoLog(shader, logLenght, &logLenght, &log[0]);

		std::cout << "Failed to compile the shader : " << std::endl;
		std::copy(log.begin(), log.end(), std::ostream_iterator<char>(std::cout));
		std::cout << std::endl;

		glDeleteShader(shader);
	}
}

// Create, compiles and attach a type of shader to the program passed in the arguments
unsigned int Shader::compileShader(const std::string& shaderSource, unsigned int shaderType, unsigned int program) {

	unsigned int shader = glCreateShader(shaderType);

	const char* vShaderCC = &shaderSource[0];
	glShaderSource(shader, 1, &vShaderCC, NULL);
	glCompileShader(shader);

	// Error checking
	compileErrorCheck(shader);

	// What happens if shader doesn't compile ? Ugly error cause we deleted the shader

	glAttachShader(program, shader);

	return shader;
}

int Shader::createShader(const std::string& vShaderS, const std::string& fShaderS) {

	unsigned int program = glCreateProgram();

	unsigned int vShader = compileShader(vShaderS, GL_VERTEX_SHADER, program);
	unsigned int fShader = compileShader(fShaderS, GL_FRAGMENT_SHADER, program);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	// glDetach ?
	return program;
}