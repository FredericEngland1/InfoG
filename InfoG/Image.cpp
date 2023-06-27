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

		imgImported = cv::imread("DefaultTexture.png", cv::IMREAD_COLOR);
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

void Image::randomImage()
{
	img = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
	cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);

	MainWConsole::mainConsole.addLog("Randomizing the diffuse map, expect a short drop in fps.", LogType::information);
	
	for (int i = 0; i < img.rows; i++)
	{
		for (int i2 = 0; i2 < img.cols; i2++)
		{
			img.at<cv::Vec4b>(i, i2)[0] = rand() % 255;
			img.at<cv::Vec4b>(i, i2)[1] = rand() % 255;
			img.at<cv::Vec4b>(i, i2)[2] = rand() % 255;
			img.at<cv::Vec4b>(i, i2)[3] = rand() % 255;
		}
	}
}
