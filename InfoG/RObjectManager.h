#pragma once
#include "MainWConsole.h"
#include "RObject.h"

#include <unordered_map>

#include <assimp/scene.h>
#include <assimp/mesh.h>

#include <assimp/Importer.hpp>

class RObjectManager {
private :

	static std::unordered_map<unsigned int, RObject> rObjects;
	static unsigned int currID;

public :

	static std::unordered_map<unsigned int, RObject>& getRObjects() { return rObjects; };

	static RObject getRObject(unsigned int ID);
	static void addRObject(RObject rObject);
	static void loadRObject(std::string path);

	static void removeRObject(unsigned int ID);
};