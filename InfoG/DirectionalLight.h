#pragma once

#include <glm/glm.hpp>
#include "Light.h"

struct DirectionalLight : public Light {

    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirectionalLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, LightType lightType = LightType::directional) : 
        direction(direction), ambient(ambient), diffuse(diffuse), specular(specular), Light(lightType) {};
};