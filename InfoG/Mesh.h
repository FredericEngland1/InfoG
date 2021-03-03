#include <vector>

#include <glm/vec3.hpp>

class Mesh {
private :

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	unsigned int meshType;

public :

	Mesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices, unsigned int meshType);

	Mesh() = default;

	std::vector<glm::vec3> getVertices() { return vertices; };
	std::vector<unsigned int> getIndices() { return indices; };
	unsigned int getMeshType() { return meshType; };
};