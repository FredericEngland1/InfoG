#pragma once

#include "Shader.h"

#include <vector>
#include <string>
#include <utility>

class ShaderManager {
private :
	static std::vector<std::pair<std::string, Shader>> shaders;
public :
	~ShaderManager();
	static Shader getShader(std::string VSPath, std::string FSPath);
};