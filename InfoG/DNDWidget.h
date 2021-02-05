#pragma once

class DNDWidget {
protected :
	bool notified = false;

public :
	DNDWidget();
	void notify();
};