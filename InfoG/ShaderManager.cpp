#include "ShaderManager.h"

std::vector<std::pair<std::string, Shader>> ShaderManager::shaders;

ShaderManager::~ShaderManager(){
    for (std::pair<std::string, Shader> shaderPair : shaders) {
        shaderPair.second.destroy();
    }
}

Shader ShaderManager::getShader(std::string VSPath, std::string FSPath, std::string GSPath)
{
    auto it = std::find_if(shaders.begin(), shaders.end(),
        [&VSPath, FSPath, GSPath](const std::pair<std::string, Shader>& element) { return !element.first.compare(VSPath + FSPath + GSPath); });

    Shader shader;

    if (it != shaders.end()) {
        shader = ((std::pair<std::string, Shader>) * it).second;
    }
    else {
        shader = Shader(VSPath, FSPath, GSPath);
        shaders.push_back(std::make_pair( VSPath + FSPath + GSPath, shader));
    }

    return shader;
}
