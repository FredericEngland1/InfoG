#pragma once

#include "DNDImgWidget.h"
#include "Mosaic.h"

class DNDMosaicWidget : public DNDImgWidget {
private :
	void changeImage(std::vector<std::string> paths);

	Mosaic img;

public :

	DNDMosaicWidget(unsigned int width = 200, unsigned int height = 200);

	void display() override;
	void outputImage();
};