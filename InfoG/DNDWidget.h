#pragma once

#include "Window.h"

class DNDWidget {
protected :
	bool notified = false;

public :
	DNDWidget();
	void notify();

	virtual void render() = 0;
};