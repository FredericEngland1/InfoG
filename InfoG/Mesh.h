#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Mesh {
	private :

		std::vector<glm::uvec3> indices;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;

	public :

		Mesh(std::vector<glm::vec3> vertices, std::vector<glm::uvec3> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uv);

		Mesh() = default;

		std::vector<glm::vec3> getVertices() { return vertices; };
		std::vector<glm::vec3> getNormals() { return normals; };
		std::vector<glm::vec2> getUV() { return uv; };
		std::vector<glm::uvec3> getIndices() { return indices; };

		glm::vec3 vMin;
		glm::vec3 vMax;
};