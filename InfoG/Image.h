#pragma once

#include <opencv2/opencv.hpp>
#include "MainWConsole.h"

class Image {
protected :
	cv::Mat img;

	unsigned int width;
	unsigned int height;

public :
	Image(unsigned int width = 200, unsigned int height = 200);
	Image(std::string path);
	void replaceImg(std::string path);

	virtual const void* getPixels() { return img.data; };
	unsigned int getWidth() { return img.cols; };
	unsigned int getHeight() { return img.rows; };

	cv::Mat getImage() { return img; };

	bool empty() { return img.empty(); };

	void flipvImage();
	void fliphImage();
	void resizeImage(unsigned int width = 200, unsigned int height = 200);

	void randomImage();
};