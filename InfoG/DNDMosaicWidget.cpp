#include "DNDMosaicWidget.h"

void DNDMosaicWidget::changeImage(std::vector<std::string> paths) {
	for (std::string path : paths) panorama.addImage(path);

	img = panorama.getMosaic();
}

void DNDMosaicWidget::display() {

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.cols, img.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
	ImGui::Image((void*)(static_cast<intptr_t>(texture)), ImVec2(img.cols, img.rows));

	if (notified) {
		if (ImGui::IsItemHovered()) {
			changeImage(DNDFileManager::getPaths());
		}
		notified = false;
	}

	if (ImGui::Button("Output mosaic")) {
		outputImage();
	}
}

void DNDMosaicWidget::outputImage() {

	cv::Mat outputImg;
	cv::cvtColor(img, outputImg, cv::COLOR_RGBA2BGRA);

	cv::imwrite("result.jpg", outputImg);
}