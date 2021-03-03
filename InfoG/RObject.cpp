#include "RObject.h"

void RObject::initialyzeMesh()
{
    unsigned int vbo, ibo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &ibo);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertices().size() * sizeof(glm::vec3), &mesh.getVertices()[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices().size() * sizeof(unsigned int), &mesh.getIndices()[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

void RObject::initialyzeTexture()
{

    switch (texture.getType())
    {
    case TextureType::plainColor:

        break;
    case TextureType::uv:

        break;
    default:
        break;
    }
    
}

void RObject::updateModelMatrix()
{
    modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::rotate(0.0f, rotation) * glm::scale(scale); // Angle ?
}

RObject::RObject(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices, unsigned int meshType, 
    std::string name, glm::vec3 position, std::string VSPath, std::string FSPath) : position(position), name(name)
{
    RObject::mesh = Mesh(vertices, indices, meshType);
    initialyze(VSPath, FSPath);
}

RObject::RObject(Mesh m, std::string name, glm::vec3 position, std::string VSPath, std::string FSPath) : mesh(m), position(position), name(name)
{
    initialyze(VSPath, FSPath);
}

void RObject::initialyze(std::string VSPath, std::string FSPath) {

    rotation = { 0,1.0f,0 };
    scale = { 1.0f,1.0f,1.0f };

    setShader(VSPath, FSPath);
    updateModelMatrix();

    initialyzeMesh();


    // Texture initialyzation


}

void RObject::render()
{
    shader.use();

    glBindVertexArray(vao);
    glDrawElements(mesh.getMeshType(), mesh.getIndices().size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind the vertex array
}

void RObject::setShader(std::string VSPath, std::string FSPath)
{
    shader = ShaderManager::getShader(VSPath, FSPath);
    MVPMatrixID = glGetUniformLocation(shader.getProgramID(), "MVP");
}

void RObject::setShader(Shader shader)
{
    RObject::shader = shader;
    MVPMatrixID = glGetUniformLocation(shader.getProgramID(), "MVP");
}
