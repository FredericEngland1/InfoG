#include "DNDFileManager.h"

#include <iostream>

std::vector<DNDWidget*> DNDFileManager::subscriberList;
std::vector<std::string> DNDFileManager::pathList;

void dropFiles(GLFWwindow* window, int count, const char** paths)
{
	// We call glfwShowWindow to focus the window before the hover checks so imgui can correctly detect the mouse hovering
	glfwShowWindow(window);

	for (int i = 0; i < count; i++) {
		DNDFileManager::addPath(paths[i]);
	}
	DNDFileManager::notifySubs();
}

void DNDFileManager::notifySubs() {
	for (DNDWidget* widget : subscriberList) {
		widget->notify();
	}
}

std::vector<std::string> DNDFileManager::getPaths() {
	return pathList;
}
void DNDFileManager::addPath(const char* path) {
	pathList.push_back(std::string(path));
}
void DNDFileManager::clearPaths() {
	pathList.clear();
}


void DNDFileManager::setGLFWwindow(GLFWwindow* window) {
	glfwSetDropCallback(window, dropFiles);
}

void DNDFileManager::subscribe(DNDWidget* widget) {
	subscriberList.push_back(widget);
}

void DNDFileManager::unSubscribe(DNDWidget* widget) {
	subscriberList.erase(std::remove(subscriberList.begin(), subscriberList.end(), widget), subscriberList.end());
}