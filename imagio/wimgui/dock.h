#pragma once

#ifndef WIMGUI_DOCK_H
#define WIMGUI_DOCK_H

#include <vector>
#include "../imgui/imgui.h"
#include "window.h"
#include "workspace.h"


namespace wimgui
{

class dock_painter;

enum dock_style {
	dock_left, dock_right, dock_top, dock_bottom, dock_fill
};

enum dock_draw_mode {
	draw_normal, draw_hovered, draw_resizing, draw_none
};

const float hover_delta = 5.0f;
const float window_extra_area = 20.0f;

class docker: public background_window {

	friend class dock_painter;
	friend class dock_left_painter;
	friend class dock_bottom_painter;
	friend class dock_top_painter;

	std::vector<window *> windows;
	dock_style style = dock_left;
	dock_painter* painter;
	bool border_hovered = false;
	bool border_held = false;

public:

	docker(const char* title);
	docker(const char* title, dock_style style);
	~docker();
	void add_window(window *);
	void remove_window(window *);
	void set_dock_style(dock_style style);

	float get_inner_width();
	float get_inner_height();

	void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position);
	void adjust(ImRect* client_window);
	void draw(dock_draw_mode mode);
	virtual void draw();
	virtual void draw_imgui();

protected:
	void set_min_width();
	void set_min_height();
};


class dock_painter
{

protected:
	docker* dock;
	int sleep = 10;

public:
	dock_painter(docker* dock);
    virtual ~dock_painter();
	virtual ImRect get_border_rectangle() = 0;
	virtual void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position) = 0;
	virtual void adjust() = 0;
	virtual void adjust(ImRect* window_area) = 0;
	virtual float get_inner_width() = 0;
	virtual float get_inner_height() = 0;
	virtual void draw_border(dock_draw_mode mode) = 0;
	virtual void draw_border2(ImColor color, ImVec2 from, ImVec2 to, float line_width);
	virtual void draw_border2(ImColor color, ImRect border_rectangle);
	virtual float border_width();
};


class dock_left_painter : public dock_painter
{

public:
	dock_left_painter(docker* dock) : dock_painter(dock) {};
	virtual ImRect get_border_rectangle();
	virtual void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position);
	virtual void adjust();
	virtual void adjust(ImRect* client_window);
	virtual float get_inner_width();
	virtual float get_inner_height();
	virtual void  draw_border(dock_draw_mode mode);
};

class dock_bottom_painter : public dock_painter
{

public:
	dock_bottom_painter(docker* dock) : dock_painter(dock) {};
	virtual ImRect get_border_rectangle();
	virtual void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position);
	virtual void adjust();
	virtual void adjust(ImRect* client_window);
	virtual float get_inner_width();
	virtual float get_inner_height();
	virtual void  draw_border(dock_draw_mode mode);
};

class dock_top_painter : public dock_painter
{

public:
	dock_top_painter(docker* dock) : dock_painter(dock) {};
	virtual ImRect get_border_rectangle();
	virtual void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position);
	virtual void adjust();
	virtual void adjust(ImRect* client_window);
	virtual float get_inner_width();
	virtual float get_inner_height();
	virtual void  draw_border(dock_draw_mode mode);
};

}

#endif
