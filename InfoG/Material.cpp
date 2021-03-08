#include "Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, std::string name, std::string diffuseMap) : 
	ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), name(name)
{
	Material::diffuseMap = Texture(diffuseMap);
}
