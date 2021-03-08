#pragma once

#include <vector>
#include "RObject.h"

class Node {
private :

	std::vector<unsigned int> rObjects;
	std::vector<Node> childNodes;

	unsigned int ID;

public :
	
	Node(std::vector<Node> childNodes, Node parentNode, std::vector<unsigned int> rObjects, unsigned int ID);
	Node(unsigned int ID);
	Node() = default;

	Node* parentNode;

	std::vector<unsigned int> getRObjects() { return rObjects; };
	std::vector<Node> getChildNodes() { return childNodes; };

	void addChildNode(Node node) { childNodes.push_back(node); };
	void removeChildNode(unsigned int cID);

	void addChildRObject(unsigned int rObject) { rObjects.push_back(rObject); };
	void removeRObject(unsigned int rObject) { rObjects.erase(std::remove(rObjects.begin(), rObjects.end(), rObject), rObjects.end()); };

	unsigned int getID() { return ID; };
};