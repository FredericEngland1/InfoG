#pragma once

#include <glm/glm.hpp>
#include "Light.h"

struct PointLight : public Light{

    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, LightType lightType = LightType::point) : 
        position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant), linear(linear), quadratic(quadratic),
        Light(lightType) {};
};