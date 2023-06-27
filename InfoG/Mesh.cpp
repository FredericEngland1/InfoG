#include "Mesh.h"

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<glm::uvec3> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uv) : vertices(vertices), indices(indices), normals(normals), uv(uv)
{
}