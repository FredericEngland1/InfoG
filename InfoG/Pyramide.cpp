#include "Pyramide.h"

Pyramide::Pyramide()
{

    std::vector<glm::vec3> vertices{
        {-.5,0,-.5},
        {.5,0,-.5},
        {-.5,0,.5},

        {0,1,0}
    };

    std::vector<glm::vec3> normals{
        {128,128,128},
        {128,128,128},
        {128,128,128},
        {128,128,128}
    };

    std::vector<glm::vec2> uv{
        {0,0},
        {0,1},

        {1,0},
        {1,1},

        {1,0},
        {0,0}
    };

    std::vector<glm::uvec3> indices{
        {0,1,2},

        {0,1,3},
        {1,2,3},

        {0,2,3}
    };



    glm::vec3 ambient{ 1,1,1 };
    glm::vec3 diffuse{ 1,1,1 };
    glm::vec3 specular{ 1,1,1 };
    float shininess = 1;

    Material material = Material(ambient, diffuse, specular, shininess, "PyramideMat", "DefaultTexture.png");

    RObject cube = RObject(vertices, indices, normals, uv, material, "Pyramide", { 0,0,0 }, "PBS.shader", "Phong.shader");

    Node node = Node(RObjectManager::currID++); // doit changer pour pas utiliser l'id et la remttre private
    node.parentNode = &node;
    unsigned int ID = RObjectManager::addRObject(cube);
    node.addChildRObject(ID);
    RObjectManager::addNode(node);
}
