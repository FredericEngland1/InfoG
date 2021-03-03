#include "RObjectManager.h"

std::unordered_map<unsigned int, RObject> RObjectManager::rObjects;
unsigned int RObjectManager::currID = 0;

RObject RObjectManager::getRObject(unsigned int ID)
{
    return rObjects.at(ID);
}

void RObjectManager::addRObject(RObject rObject)
{
    rObject.setID(currID);
    rObjects.insert(std::make_pair(currID, rObject));

    currID++;
}

void RObjectManager::loadRObject(std::string path)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, NULL);

	if (!scene) MainWConsole::mainConsole.addLog("Could not load the object", LogType::error);

    scene->mMeshes;
}

void RObjectManager::removeRObject(unsigned int ID)
{
    rObjects.erase(ID);
}
