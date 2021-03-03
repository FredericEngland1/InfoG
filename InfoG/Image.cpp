#include "Image.h"

Image::Image(unsigned int width, unsigned int height) : width(width), height(height) {

	img = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
	cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);

}

void Image::replaceImg(std::string path) {
	cv::Mat imgImported = cv::imread(path, cv::IMREAD_COLOR);

	if (imgImported.empty()) {
		std::cout << "Failed to load image : " << std::endl;
		std::cout << path << std::endl;

		return;
	}

	cv::cvtColor(imgImported, imgImported, cv::COLOR_BGR2RGBA);

	cv::Size size(width, height);
	cv::Mat resizedImg;
	cv::resize(imgImported, resizedImg, size);

	img = resizedImg;
}