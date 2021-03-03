#include "DNDMosaicWidget.h"

DNDMosaicWidget::DNDMosaicWidget(unsigned int width, unsigned int height) {
	img = Mosaic(width, height);
}

void DNDMosaicWidget::changeImage(std::vector<std::string> paths) {
	for (std::string path : paths) img.addImage(path);
}

void DNDMosaicWidget::render() {

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixels());
	ImGui::Image((void*)(static_cast<intptr_t>(texture)), ImVec2(img.getWidth(), img.getHeight()));

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

	img.outputImage("Result.jpg");

}