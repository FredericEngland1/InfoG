#pragma once
#include "MainWConsole.h"
#include "RObject.h"
#include "Material.h"
#include "Image.h"
#include "Node.h"
#include "Light.h"

#include "PerspectiveCamera.h"
#include "Camera.h"

#include <unordered_map>

#include <assimp/scene.h>
#include <assimp/mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class RObjectManager {
private :

	static std::unordered_map<unsigned int, Light*> lights;
	static std::unordered_map<unsigned int, Camera*> cameras;
	static std::unordered_map<unsigned int, Node> nodes;
	static std::unordered_map<unsigned int, RObject> rObjects;


	static std::vector<unsigned int> nodesToDelete;

	static void removeRObjectFromNodes(Node &node, unsigned int ID);
	static void removeAllRObjectFromNodes(Node &node);
	static void removeRObjectFromNodes(unsigned int ID);

public :

	static unsigned int currID;

	static std::unordered_map<unsigned int, RObject>& getRObjects() { return rObjects; };
	static std::unordered_map<unsigned int, Node>& getNodes() { return nodes; };
	static std::unordered_map<unsigned int, Light*>& getLights() { return lights; };
	static std::unordered_map<unsigned int, Camera*>& getCameras() { return cameras; };

	static RObject getRObject(unsigned int ID);
	static unsigned int addRObject(RObject rObject);
	static void loadRObject(std::string path);
	static void loadImage(std::string path);

	static void removeRObject(unsigned int ID);

	static void addNode(Node node);
	static void removeNode(unsigned int ID);

	static void addLight(Light* light);
	static void removeLight(unsigned int ID);

	static void addCamera(Camera* camera);
	static void removeCamera(unsigned int ID);

	static std::vector<unsigned int> selectedIds;
};