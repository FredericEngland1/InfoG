#pragma once

#include <imgui.h>

class Window {
public :
	virtual void render() = 0;
	bool p_open = true;
};