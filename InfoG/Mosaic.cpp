#include "Mosaic.h"

Mosaic::Mosaic(unsigned int width, unsigned int height, unsigned int borderTickness) : Image(width, height) {
	Mosaic::borderTickness = borderTickness;
}

void Mosaic::addImage(std::string path) {
	
	cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);

	if (img.empty()) {
		std::cout << "Failed to load image to panorama : " << std::endl;
		std::cout << path << std::endl;

		return;
	}

	cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);

	cv::Size size(width, height);
	cv::Mat resizedImg;
	cv::resize(img, resizedImg, size);

	images.push_back(resizedImg);
	requiresCreation = true;
}

void Mosaic::clearImages() {
	images.clear();
}

void Mosaic::createMosaic() {

	cv::Mat mosaicImg;
	cv::Mat tempMosaicImg;

	for (int i = 1; i <= images.size(); i++) {
		cv::Mat tempImg;
		cv::copyMakeBorder(images[i-1], tempImg, borderTickness, borderTickness, borderTickness, borderTickness, cv::BORDER_CONSTANT, { 0,0,0,255 });

		if (tempMosaicImg.empty()) {
			tempMosaicImg = tempImg;
		}
		else {
			cv::hconcat(tempMosaicImg, tempImg, tempMosaicImg);

			if (i == 3) {
				mosaicImg = tempMosaicImg;
				tempMosaicImg = cv::Mat();
			}
			else if (i%3 == 0) {
				cv::vconcat(mosaicImg, tempMosaicImg, mosaicImg);
				tempMosaicImg = cv::Mat();
			}
		}
	}

	if (!tempMosaicImg.empty() && !mosaicImg.empty()) {
		cv::Mat blankImg = cv::Mat::zeros(cv::Size(220, 220), tempMosaicImg.type());
		for (int i = 0; i < 3 - (images.size() % 3); i++) {
			cv::hconcat(tempMosaicImg, blankImg, tempMosaicImg);
		}
		cv::vconcat(mosaicImg, tempMosaicImg, mosaicImg);
	}

	if (mosaicImg.empty()) img = tempMosaicImg;
	else img = mosaicImg;
}

const void* Mosaic::getPixels() {

	if (requiresCreation) {
		createMosaic();
		requiresCreation = false;
	}

	if (img.empty()) {
		std::cout << "Trying to get an empty panorama" << std::endl;
	}

	return img.data;
}

void Mosaic::outputImage(std::string path) {
	cv::Mat outputImg;
	cv::cvtColor(img, outputImg, cv::COLOR_RGBA2BGRA);

	cv::imwrite("result.jpg", outputImg);
}