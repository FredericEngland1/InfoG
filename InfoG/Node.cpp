#include "Node.h"

Node::Node(std::vector<Node> childNodes, Node parentNode, std::vector<unsigned int> rObjects, unsigned int ID) : childNodes(childNodes), parentNode(&parentNode), rObjects(rObjects), ID(ID)
{
}

Node::Node(unsigned int ID) : ID(ID)
{
}

void Node::removeChildNode(unsigned int cID)
{
	for (std::vector<Node>::iterator it = childNodes.begin(); it != childNodes.end(); it++) {
		if (it->getID() == cID) {
			childNodes.erase(it);
		}
	}
}
