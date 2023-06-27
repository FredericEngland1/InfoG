#pragma once

#include <glm/glm.hpp>
#include "Light.h"
#include "RObjectManager.h"

class PointLight: public Light{
public:
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    PointLight(bool add = true, glm::vec3 position = { 0, 0, 0 }, glm::vec3 ambient = { 0, 0, 0 }, glm::vec3 diffuse = { 1, 1, 1 }, glm::vec3 specular = { 1, 1, 1 }, float constant = 1, float linear = 0, float quadratic = 0, LightType lightType = LightType::point);
};