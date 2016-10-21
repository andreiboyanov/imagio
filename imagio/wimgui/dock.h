#pragma once

#ifndef WIMGUI_DOCK_H
#define WIMGUI_DOCK_H

#include <vector>
#include "../imgui/imgui.h"
#include "window.h"
#include "common.h"


namespace wimgui
{

enum dock_style {
	dock_left, dock_right, dock_up, dock_down, dock_fill
};

enum dock_draw_mode {
	draw_normal, draw_resizing, draw_none
};

const float hoover_delta = 5.0f;

class docker {

	int sleep = 10;
	std::vector<window *> windows;
	dock_style style = dock_left;
	ImVec2 size = ImVec2(0.0f, 0.0f);
	ImVec2 position = ImVec2(0.0f, 0.0f);
	window* parent;

public:

	docker(window* parent_window);
	docker(window* parent_window, dock_style style);
	~docker();
	void add_window(window *);
	void remove_window(window *);
	ImVec2 preferred_size(float width = -1.0f, float height = -1.0f);
	ImVec2 preferred_position(float x = -1.0f, float y = -1.0f);
	void draw(dock_draw_mode mode);
	void draw();
	void adjust();
	void adjust(window_area* client_window);

protected:
	void virtual draw_border(ImColor color);
};

}

#endif