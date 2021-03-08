#include "PlusSign.h"

PlusSign::PlusSign()
{
    std::vector<glm::vec3> vertices{
       {0,0,0},
       {0.5,0,0},

       {-.5,.5,0},
       {0,.5,0},
       {.5,.5,0},
       {1,.5,0},

       {-.5,1,0},
       {0,1,0},
       {.5,1,0},
       {1,1,0},

       {0,1.5,0},
       {0.5,1.5,0},
    };

    std::vector<glm::vec3> normals{
        {128,128,128},
        {128,128,128},
        {128,128,128},
        {128,128,128},

        {128,128,128},
        {128,128,128},
        {128,128,128},
        {128,128,128}
    };

    std::vector<glm::vec2> uv{
        {0,0},
        {1,1}
    };

    std::vector<glm::uvec3> indices{
        {0,1,4},
        {0,3,4},

        {2,5,9},
        {2,6,9},

        {7,8,11},
        {7,10,11},
    };



    glm::vec3 ambient{ 1,1,1 };
    glm::vec3 diffuse{ 1,1,1 };
    glm::vec3 specular{ 1,1,1 };
    float shininess = 1;

    Material material = Material(ambient, diffuse, specular, shininess, "PlusSignMat", "DefaultTexture.png");

    RObject cube = RObject(vertices, indices, normals, uv, material, "PlusSign", { 0,0,0 }, "PBS.shader", "Phong.shader");

    Node node = Node(RObjectManager::currID++); // doit changer pour pas utiliser l'id et la remttre private
    node.parentNode = &node;
    unsigned int ID = RObjectManager::addRObject(cube);
    node.addChildRObject(ID);
    RObjectManager::addNode(node);
}
