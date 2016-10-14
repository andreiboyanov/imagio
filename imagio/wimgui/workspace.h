#pragma once

#ifndef WIMGUI_WORKSPACE_H
#define WIMGUI_WORKSPACE_H


#include <vector>
#include "window.h"
#include "dock.h"

namespace wimgui {

class workspace {

	std::vector<window *> windows;
	std::vector<docker *> docks;

public:
	workspace();
	~workspace();

	void add_window(window *window);
	void add_dock(docker *dock);
	void draw();
};

}

#endif