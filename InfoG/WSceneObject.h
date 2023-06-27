#pragma once

#include "Window.h"
#include "DNDWidget.h"
#include "RObjectManager.h"
#include "DNDFileManager.h"
#include "Node.h"

#include "RObject.h"

class WSceneObject : public Window, public DNDWidget {
private :

	void renderChildNodes(Node node);

	void renderLights(Light light);

public :

	void render() override;

};