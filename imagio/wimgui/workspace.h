#pragma once

#ifndef WIMGUI_WORKSPACE_H
#define WIMGUI_WORKSPACE_H


#include <vector>
#include "../imgui/imgui.h"
#include "window.h"
#include "dock.h"
#include "common.h"

namespace wimgui {

class workspace: public background_window {

	std::vector<window *> windows;
	std::vector<docker *> docks;

public:
	workspace();
	~workspace();

	void add_window(window *window);
	void add_dock(docker *dock);
	window_area* get_client_area();
	void draw();
	void draw_workspace();

};


}

#endif
