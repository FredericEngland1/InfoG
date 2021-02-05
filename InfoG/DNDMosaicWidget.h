#pragma once

#include "DNDImgWidget.h"
#include "Mosaic.h"

class DNDMosaicWidget : public DNDImgWidget {
private :
	void changeImage(std::vector<std::string> paths);

	Mosaic panorama = Mosaic();

public :
	void display() override;

	void outputImage();
};