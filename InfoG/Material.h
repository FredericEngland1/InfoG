#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Texture.h"

class Material {
public :

    Material(glm::vec3 ambient = {1,1,1}, glm::vec3 diffuse = { 1,1,1 }, glm::vec3 specular = { 1,1,1 }, float shininess = 30, std::string name = "DefaultMaterial", std::string diffuseMap = "");

    // TODO might not want everything public

    std::string name;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Texture diffuseMap;

    /*std::string normalMap;
    std::string diffuseMap;
    std::string diffuseMap;
    std::string diffuseMap;
    std::string diffuseMap;
    std::string diffuseMap;*/

    float shininess;
};