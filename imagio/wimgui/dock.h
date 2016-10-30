#pragma once

#ifndef WIMGUI_DOCK_H
#define WIMGUI_DOCK_H

#include <vector>
#include "../imgui/imgui.h"
#include "window.h"
#include "common.h"


namespace wimgui
{

class dock_painter;

enum dock_style {
	dock_left, dock_right, dock_up, dock_down, dock_fill
};

enum dock_draw_mode {
	draw_normal, draw_hovered, draw_resizing, draw_none
};

const float hover_delta = 5.0f;
const float window_extra_area = 20.0f;

class docker: public background_window {

	friend class dock_painter;
	friend class dock_left_painter;

	std::vector<window *> windows;
	dock_style style = dock_left;
	dock_painter* painter;

public:

	docker(const char* title);
	docker(const char* title, dock_style style);
	~docker();
	void add_window(window *);
	void remove_window(window *);
	void set_dock_style(dock_style style);

	float get_inner_width();
	float get_inner_height();

	void adjust(window_area* client_window);
	void draw(dock_draw_mode mode);
	virtual void draw();
	virtual void draw_imgui();

protected:
	void virtual draw_border(ImColor color, float line_width = 1.0f);
	void set_min_width();
};


class dock_painter
{

protected:
	docker* dock;
	int sleep = 10;

public:
	dock_painter(docker* dock);
    virtual ~dock_painter();
	virtual ImRect get_border_rectangle() { return ImRect();  }
	virtual void adjust() {}
	virtual void adjust(window_area*) { }
};


class dock_left_painter : public dock_painter
{

public:
	dock_left_painter(docker* dock);
	virtual ImRect get_border_rectangle();
	virtual void adjust();
	virtual void adjust(window_area* client_window);
};

}

#endif
