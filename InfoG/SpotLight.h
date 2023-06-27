#pragma once

#include <glm/glm.hpp>

#include "PointLight.h"

class SpotLight : public PointLight {
public :
    glm::vec3 direction;

    float cutOff;
    float outerCutOff;

    SpotLight(glm::vec3 direction = { 0.5,-1,-1 }, float cutOff = 1, float outerCutOff = 1000, glm::vec3 position = {20, 20, 0}, glm::vec3 ambient = { 0, 0, 0 }, glm::vec3 diffuse = { 0.66,0.66,0.66 }, glm::vec3 specular = { 0.1,0.1,0.1 }, float constant = 1, float linear = 0, float quadratic = 0, LightType lightType = LightType::spot) :
        direction(direction), cutOff(cutOff), outerCutOff(outerCutOff),
        PointLight(false, position, ambient, diffuse, specular, constant, linear, quadratic, lightType) {
        RObjectManager::addLight(this);
    };
};