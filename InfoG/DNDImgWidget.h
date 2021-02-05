#pragma once

#include <string>
#include <iostream>

#include <imgui.h>

#include <GL/gl3w.h>

#include <opencv2/opencv.hpp>

#include "DNDWidget.h"
#include "DNDFileManager.h"

class DNDImgWidget : public DNDWidget {
protected :
	virtual void changeImage(std::string path);

	unsigned int width;
	unsigned int height;

	cv::Mat img;
public :
	DNDImgWidget(unsigned int width = 200, unsigned int height = 200);
	virtual void display();
};