#include "RObject.h"

void RObject::initialyzeMesh()
{
    unsigned int vbo, vnbo, vtbo, ibo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &ibo);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vnbo);
    glGenBuffers(1, &vtbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.getVertices().size() * sizeof(glm::vec3), &mesh.getVertices()[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vnbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.getNormals().size() * sizeof(glm::vec3), &mesh.getNormals()[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vtbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.getUV().size() * sizeof(glm::vec2), &mesh.getUV()[0], GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndices().size() * sizeof(glm::uvec3), &mesh.getIndices()[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

void RObject::updateModelMatrix()
{
    modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::rotate(degreeRotation, rotation) * glm::scale(scale); // Angle ?
}

RObject::RObject(std::vector<glm::vec3> vertices, std::vector<glm::uvec3> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uv, Material material,
    std::string name, glm::vec3 position, std::string VSPath, std::string FSPath) : position(position), name(name), material(material)
{
    RObject::mesh = Mesh(vertices, indices, normals, uv);
    initialyze(VSPath, FSPath);
}

RObject::RObject(Mesh m, std::string name, glm::vec3 position, std::string VSPath, std::string FSPath) : mesh(m), position(position), name(name)
{
    initialyze(VSPath, FSPath);
}

void RObject::initialyze(std::string VSPath, std::string FSPath) {

    meshImageIDIn = 0;
    meshImageIDOut = 0;

    degreeRotation = 0;
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
    glDrawElements(GL_TRIANGLES, mesh.getIndices().size() * sizeof(glm::uvec3), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // Unbind the vertex array
}

void RObject::useShader()
{
    shader.use();
}

void RObject::setShader(std::string VSPath, std::string FSPath, std::string GSPath)
{
    shader = ShaderManager::getShader(VSPath, FSPath, GSPath);

    RObject::VSPath = VSPath;
    RObject::FSPath = FSPath;
    RObject::GSPath = GSPath;

    MVMatrixID = getUniformLocation("MV");
    MVPMatrixID = getUniformLocation("MVP");
    normalMatrixID = getUniformLocation("normalMatrix");
}

void RObject::setShader(Shader shader)
{
    RObject::shader = shader;
    MVMatrixID = getUniformLocation("MV");
    MVPMatrixID = getUniformLocation("MVP");
    normalMatrixID = getUniformLocation("cameraPosition");
}

unsigned int RObject::getUniformLocation(std::string location)
{
    return glGetUniformLocation(shader.getProgramID(), location.c_str());
}