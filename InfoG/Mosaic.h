#pragma once

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <cmath>

class Mosaic {
private :
	void createMosaic();

	std::vector<cv::Mat> images;
	cv::Mat mosaic;
	bool requiresCreation = false;

	unsigned int width;
	unsigned int height;
	unsigned int borderTickness;

public :
	Mosaic(unsigned int width = 200, unsigned int height = 200, unsigned int borderTickness = 10);

	void addImage(std::string path);
	void clearImages();

	cv::Mat getMosaic();
};