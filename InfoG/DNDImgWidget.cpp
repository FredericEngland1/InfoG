#include "DNDImgWidget.h"

DNDImgWidget::DNDImgWidget(unsigned int width, unsigned int height) {
	img = cv::Mat::zeros(cv::Size(width, height), CV_8UC1);
	cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);

	DNDImgWidget::width = width;
	DNDImgWidget::height = height;
}

void DNDImgWidget::display() {

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
	ImGui::Image((void*)(static_cast<intptr_t>(texture)), ImVec2(img.cols, img.rows));

	if (notified) {
		if (ImGui::IsItemHovered()) changeImage(DNDFileManager::getPaths().front());
		notified = false;
	}

}

void DNDImgWidget::changeImage(std::string path) {
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