#pragma once

#include <opencv2/opencv.hpp>

class Image {
protected :
	cv::Mat img;

	unsigned int width;
	unsigned int height;

public :
	Image(unsigned int width = 200, unsigned int height = 200);
	void replaceImg(std::string path);

	virtual const void* getPixels() { return img.data; };
	unsigned int getWidth() { return img.cols; };
	unsigned int getHeight() { return img.rows; };
};