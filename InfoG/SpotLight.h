#pragma once

#include <glm/glm.hpp>

#include "PointLight.h"

struct SpotLight : public PointLight {

    glm::vec3 direction;

    float cutOff;
    float outerCutOff;

    SpotLight(glm::vec3 direction, float cutOff, float outerCutOff, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, LightType lightType = LightType::spot) :
        direction(direction), cutOff(cutOff), outerCutOff(outerCutOff),
        PointLight(position, ambient, diffuse, specular, constant, linear, quadratic, lightType) {};
};