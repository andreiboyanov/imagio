#pragma once

#ifndef WIMGUI_DOCK_H
#define WIMGUI_DOCK_H

#include "vector"
#include "../imgui/imgui.h"
#include "window.h"

namespace wimgui
{

enum dock_style {
	dock_left, dock_right, dock_up, dock_down, dock_fill
};

enum dock_draw_mode {
	draw_normal, draw_resizing, draw_none
};

class docker {

	std::vector<window *> windows;
	dock_style style = dock_left;
	ImVec2 size = ImVec2(0.0f, 0.0f);

public:

	docker();
	docker(dock_style style);
	~docker();
	void add_window(window *);
	void remove_window(window *);
	ImVec2 preferred_size();
	void draw(dock_draw_mode mode);
	void adjust_size();

};

}

#endif