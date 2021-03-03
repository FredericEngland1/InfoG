#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices, unsigned int meshType) : vertices(vertices), indices(indices), meshType(meshType)
{
}