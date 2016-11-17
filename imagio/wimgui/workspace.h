#pragma once

#ifndef WIMGUI_WORKSPACE_H
#define WIMGUI_WORKSPACE_H


#include <vector>
#include "../imgui/imgui.h"
#include "window.h"
#include "dock.h"

namespace wimgui {

class docker;

class workspace: public background_window {

	std::vector<window *> windows;
	std::vector<docker *> docks;

public:
	workspace();
	workspace(const char* title);
	~workspace();

	void add_window(window *window);
	void add_window(window *window, docker* dock);
	void add_dock(docker *dock);
	ImRect* get_client_area();
	void draw();
	void draw_workspace();
	void check_undocking(window* _window);
	void check_docking(window* _window);
};


}

#endif
