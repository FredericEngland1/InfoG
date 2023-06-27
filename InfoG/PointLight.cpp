#include "PointLight.h"

PointLight::PointLight(bool add, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, LightType lightType) :
    position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant), linear(linear), quadratic(quadratic),
    Light(lightType) {
    if (add) {
        RObjectManager::addLight(this);
    }
};
