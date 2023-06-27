#pragma once

#include <glm/glm.hpp>
#include "Light.h"

class DirectionalLight : public Light {
public :
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionalLight(glm::vec3 direction = { 0.5,-1,-1 }, glm::vec3 ambient = { 0, 0, 0 }, glm::vec3 diffuse = { 0.66,0.66,0.66 }, glm::vec3 specular = { 0.1,0.1,0.1 }, LightType lightType = LightType::directional) :
        direction(direction), ambient(ambient), diffuse(diffuse), specular(specular), Light(lightType) {
        RObjectManager::addLight(this);
    };
};