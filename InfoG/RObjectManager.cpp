#include "RObjectManager.h"

std::unordered_map<unsigned int, RObject> RObjectManager::rObjects;
std::unordered_map<unsigned int, Node> RObjectManager::nodes;

std::unordered_map<unsigned int, Light*> RObjectManager::lights;
std::unordered_map<unsigned int, Camera*> RObjectManager::cameras;

unsigned int RObjectManager::currID = 0;

std::vector<unsigned int> RObjectManager::selectedIds;
std::vector<unsigned int> RObjectManager::nodesToDelete;

RObject RObjectManager::getRObject(unsigned int ID)
{
    return rObjects.at(ID);
}

unsigned int RObjectManager::addRObject(RObject rObject)
{
    rObject.setID(currID);
    rObjects.insert(std::make_pair(currID, rObject));

    return currID++;
}

void RObjectManager::loadRObject(std::string path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path,
        aiProcess_GenSmoothNormals |
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType
    );

    if (!scene) {
        MainWConsole::mainConsole.addLog("Could not load the object", LogType::information);
        MainWConsole::mainConsole.addLog(importer.GetErrorString(), LogType::information);
        loadImage(path);
        return;
    }

    // add all the objects under an unbrella
    // add all the *scene->mMaterials[i], *scene->mTextures[i]

    std::vector<Material> materials;

    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {

        int shadingModel;
        scene->mMaterials[i]->Get(AI_MATKEY_SHADING_MODEL, shadingModel);

        /*if (shadingModel != aiShadingMode_Phong && shadingModel != aiShadingMode_Gouraud)
        {
            MainWConsole::mainConsole.addLog("This mesh's shading model is not implemented in this loader, setting to default material", LogType::information);

            Material mat = Material();
            materials.push_back(mat);
        }
        else {*/

            std::string name = scene->mMaterials[i]->GetName().C_Str();

            if (name.empty()) {
                name = "DefaultMatName";
            }

            // TODO verify the default value

            glm::vec3 ambient{ 1,1,1 };
            glm::vec3 diffuse{ 1,1,1 };
            glm::vec3 specular{ 1,1,1 };
            float shininess = 100;

            scene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
            scene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
            scene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, specular);
            scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess);

            aiString texturePath;
            if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
            }
            /*for (int i2 = 0; i2 < scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE); i2++) {
                aiString texturePath;
                scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, i2, &texturePath);
                MainWConsole::mainConsole.addLog(texturePath.C_Str(), LogType::information);
            }*/

            std::string pathWithoutModel = path.substr(0, path.find_last_of('\\') + 1);

            Material mat = Material(ambient, diffuse, specular, shininess, name, pathWithoutModel + texturePath.C_Str());
            materials.push_back(mat);
        //}
    }

    int startPosition = path.find_last_of('\\') + 1;
    std::string objectName = path.substr(path.find_last_of('\\') + 1, path.find_last_of('.') - startPosition);

    Node node = Node(currID++);
    node.parentNode = &node;

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv;
        std::vector<glm::uvec3> indices;

        int vertindexoffset = vertices.size();

        for (unsigned int i2 = 0; i2 < scene->mMeshes[i]->mNumVertices; i2++)
        {
            aiVector3D& vec = scene->mMeshes[i]->mVertices[i2];
            aiVector3D& norm = scene->mMeshes[i]->mNormals[i2];
            aiVector3D& textCoords = scene->mMeshes[i]->mTextureCoords[0][i2]; // What if no text coords ?

            vertices.push_back({ vec.x, vec.y, vec.z });
            normals.push_back({ norm.x, norm.y, norm.z });
            uv.push_back({ textCoords.x, textCoords.y });
        }

        for (unsigned int i2 = 0; i2 < scene->mMeshes[i]->mNumFaces; i2++)
        {
            aiFace* face = &scene->mMeshes[i]->mFaces[i2];

            indices.push_back({face->mIndices[0], face->mIndices[1], face->mIndices[2]});
        }

        unsigned int materialIndex = scene->mMeshes[i]->mMaterialIndex;

        RObject rObject = RObject(vertices, indices, normals, uv, materials[materialIndex], objectName + std::to_string(i), {0,0,0}, "PBS.shader", "Phong.shader");
        unsigned int ID = addRObject(rObject);
        node.addChildRObject(ID);
    }

    addNode(node);
}

void RObjectManager::loadImage(std::string path) {
    Image image = Image(path);

    if (image.empty()) {
        return;
    }
    
    MainWConsole::mainConsole.addLog("Transforming Image to plane", LogType::information);

    std::vector<glm::vec3> vertices{
        {0,0,0},
        {(float)image.getWidth()/100,0,0},
        {0,(float)image.getHeight()/100,0},
        {(float)image.getWidth()/100,(float)image.getHeight()/100,0}
    };

    std::vector<glm::uvec3> indices{
        {0,2,1},
        {2,1,3}
    };

    std::vector<glm::vec3> normals{
        {128,128,128},
        {128,128,128},
        {128,128,128},
        {128,128,128}
    };

    std::vector<glm::vec2> uv{
        {0,0},
        {1,0},
        {0,1},
        {1,1},
        {0,0},
        {1,0},
        {0,1},
        {1,1}
    };

    glm::vec3 ambient{ 1,1,1 };
    glm::vec3 diffuse{ 1,1,1 };
    glm::vec3 specular{ 1,1,1 };
    float shininess = 1;

    int startPosition = path.find_last_of('\\') + 1;
    std::string objectName = path.substr(path.find_last_of('\\') + 1, path.find_last_of('.') - startPosition);

    Material material = Material(ambient, diffuse, specular, shininess, objectName, path);

    RObject rObject = RObject(vertices, indices, normals, uv, material, objectName, { 0,0,0 }, "PBS.shader", "Phong.shader");

    Node node = Node(currID++);
    node.parentNode = &node;
    unsigned int ID = addRObject(rObject);
    node.addChildRObject(ID);
    addNode(node);
}

void RObjectManager::removeRObject(unsigned int ID)
{
    rObjects.erase(ID);
    removeRObjectFromNodes(ID);
}



void RObjectManager::addNode(Node node)
{
    nodes.insert(std::make_pair(node.getID(), node));
}

void RObjectManager::removeNode(unsigned int ID)
{
    removeAllRObjectFromNodes(nodes[ID]);
    nodes.erase(ID);
}

void RObjectManager::removeRObjectFromNodes(unsigned int ID)
{
    for (auto& [key, node] : nodes) {
        removeRObjectFromNodes(node, ID);
    }

    for (unsigned int toDelete : nodesToDelete) {
        removeNode(toDelete);
    }
}

void RObjectManager::removeRObjectFromNodes(Node& child, unsigned int ID)
{
    for (Node node : child.getChildNodes()) {
        removeRObjectFromNodes(node, ID);
    }

    child.removeRObject(ID);
    if (child.getRObjects().size() == 0) nodesToDelete.push_back(child.getID());
}

void RObjectManager::removeAllRObjectFromNodes(Node& child)
{
    for (Node node : child.getChildNodes()) {
        removeAllRObjectFromNodes(node);
    }
    for (unsigned int ID : child.getRObjects()) {
        removeRObject(ID);
    }
}

void RObjectManager::addLight(Light* light)
{
    unsigned int id = currID++;
    light->setID(id);
    lights.insert(std::make_pair(id, light));
}

void RObjectManager::removeLight(unsigned int ID)
{
    lights.erase(ID);
}

void RObjectManager::addCamera(Camera *camera)
{
    cameras.insert(std::make_pair(currID++, camera));
}

void RObjectManager::removeCamera(unsigned int ID)
{
    cameras.erase(ID);
}
