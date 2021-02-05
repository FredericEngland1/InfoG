#pragma once

#include <string>
#include <iostream>

#include <imgui.h>

#include <GL/gl3w.h>

#include "DNDWidget.h"
#include "DNDFileManager.h"
#include "Image.h"

class DNDImgWidget : public DNDWidget {
protected :
	virtual void changeImage(std::string path);

	Image img;

public :
	DNDImgWidget(unsigned int width = 200, unsigned int height = 200);
	virtual void display();
};