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
const float window_extra_area = 20.0f;

class docker: public background_window {

	int sleep = 10;
	std::vector<window *> windows;
	dock_style style = dock_left;

public:

	docker(const char* title);
	docker(const char* title, dock_style style);
	~docker();
	void add_window(window *);
	void remove_window(window *);

	float get_inner_width();
	float get_inner_height();

	void draw(dock_draw_mode mode);
	virtual void draw();
	virtual void draw_imgui();
	void adjust();
	void adjust(window_area* client_window);

protected:
	void virtual draw_border(ImColor color, float line_width = 1.0f);
};

}

#endif