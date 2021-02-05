#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#include "DNDWidget.h"

class DNDFileManager {
private :

	static std::vector<DNDWidget*> subscriberList;
	static std::vector<std::string> pathList;

public:
	static void setGLFWwindow(GLFWwindow* window);
	static std::vector<std::string> getPaths();
	static void addPath(const char* path);
	static void clearPaths();

	static void notifySubs();

	static void subscribe(DNDWidget* widget);
	static void unSubscribe(DNDWidget* widget);
};