#include "Image.h"

Image::Image(unsigned int width, unsigned int height) : width(width), height(height) {

	img = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
	cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);

}

Image::Image(std::string path)
{
	replaceImg(path);
}

void Image::replaceImg(std::string path) {
	cv::Mat imgImported = cv::imread(path, cv::IMREAD_COLOR);

	if (imgImported.empty()) {

		MainWConsole::mainConsole.addLog("Failed to load image : " + path, LogType::error);

		return;
	}

	cv::cvtColor(imgImported, imgImported, cv::COLOR_BGRA2RGBA);

	img = imgImported;
	height = img.rows;
	width = img.cols;
}

void Image::flipvImage()
{
	cv::flip(img, img, 1);
}

void Image::fliphImage()
{
	cv::flip(img, img, 0);
}

void Image::resizeImage(unsigned int width, unsigned int height)
{
	cv::Size size(width, height);
	cv::Mat resizedImg;
	cv::resize(img, resizedImg, size);

	img = resizedImg;
}
