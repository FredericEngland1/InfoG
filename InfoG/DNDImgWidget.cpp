#include "DNDImgWidget.h"

DNDImgWidget::DNDImgWidget(unsigned int width, unsigned int height) {
	img = Image(width, height);
}

void DNDImgWidget::display() {

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixels());
	ImGui::Image((void*)(static_cast<intptr_t>(texture)), ImVec2(img.getWidth(), img.getHeight()));

	if (notified) {
		if (ImGui::IsItemHovered()) changeImage(DNDFileManager::getPaths().front());
		notified = false;
	}

}

void DNDImgWidget::changeImage(std::string path) {

	img.replaceImg(path);
}