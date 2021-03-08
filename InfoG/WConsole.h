#pragma once

#include <vector>
#include <string>

#include "Window.h"

enum LogType
{
	information,
	error
};

class WConsole : public Window {
private:

	std::vector<std::pair<std::string, LogType>> logs;
	std::string wName;

	ImGuiTextFilter textFilter;

public:

	WConsole(std::string wName = "Console");

	void render() override;

	void addLog(std::string log, LogType logType = information);

	void removeLog(std::string log);
	void removeLog(unsigned int logNum);

	void clearLog();
};