#include "ShaderManager.h"

std::vector<std::pair<std::string, Shader>> ShaderManager::shaders;

ShaderManager::~ShaderManager(){
    for (std::pair<std::string, Shader> shaderPair : shaders) {
        shaderPair.second.destroy();
    }
}

Shader ShaderManager::getShader(std::string VSPath, std::string FSPath)
{
    auto it = std::find_if(shaders.begin(), shaders.end(),
        [&VSPath, FSPath](const std::pair<std::string, Shader>& element) { return element.first == VSPath + FSPath; });

    Shader shader;

    if (it != shaders.end()) {
        shader = ((std::pair<std::string, Shader>) * it).second;
    }
    else {
        shader = Shader(VSPath, FSPath);
        shaders.push_back(std::make_pair( VSPath + FSPath, shader));
    }

    return shader;
}
