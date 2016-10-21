#pragma once

#ifndef WIMGUI_WORKSPACE_H
#define WIMGUI_WORKSPACE_H


#include <vector>
#include "../imgui/imgui.h"
#include "window.h"
#include "dock.h"
#include "common.h"

namespace wimgui {

static const char* background_window_name = "##WORKSPACE_WINDOW";

class workspace: public window {

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

private:
	void set_background_window();
};


}

#endif