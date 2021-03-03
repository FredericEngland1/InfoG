#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <cmath>

#include "Image.h"

class Mosaic : public Image {
private :
	void createMosaic();

	std::vector<cv::Mat> images;
	bool requiresCreation = false;
	unsigned int borderTickness;

public :
	Mosaic(unsigned int width = 200, unsigned int height = 200, unsigned int borderTickness = 10);

	void addImage(std::string path);
	void clearImages();

	void outputImage(std::string path);

	virtual const void* getPixels() override;
};