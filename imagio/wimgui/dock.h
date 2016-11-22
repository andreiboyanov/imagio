#pragma once

#ifndef WIMGUI_DOCK_H
#define WIMGUI_DOCK_H

#include <list>
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

class docker: public background_window {

	friend class dock_painter;
	friend class dock_vertical_painter;
	friend class dock_left_painter;
	friend class dock_right_painter;
	friend class dock_horizontal_painter;
	friend class dock_top_painter;
	friend class dock_bottom_painter;
	friend class dock_fill_painter;

	std::list<window *> windows;
	dock_style style = dock_left;
	dock_painter* painter;
	bool border_hovered = false;
	bool border_held = false;
	bool fill = true;
	bool accept_new = true;
	dock_draw_mode current_draw_mode = draw_normal;
	int collapsed_windows = 0;

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
	inline void fill_free_space(bool _fill) { fill = _fill; }
	inline bool fill_free_space() { return fill;  }
	inline void accept_new_windows(bool accept) { accept_new = accept; }
	inline bool accept_new_windows() { return accept_new;  }
	virtual void draw();
	virtual void draw_imgui();
	window* last_visible_window();
	

protected:
	void set_min_width();
	void set_min_height();
};


class dock_painter
{

protected:
	docker* dock;
	int sleep = 10;
    float current_tabtitle_offset = 0.0f;

public:
	dock_painter(docker* dock);
    virtual ~dock_painter();
	virtual ImRect get_outer_rectangle();
	virtual ImRect get_tabbar_rectangle() = 0;
	virtual ImRect get_border_rectangle() = 0;
	virtual ImRect get_inner_rectangle() = 0;
	virtual float get_tabbar_height(bool always_visible = false);
	virtual float get_inner_width() = 0;
	virtual float get_inner_height() = 0;
	virtual void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position) = 0;
	virtual void adjust() = 0;
	virtual void adjust(ImRect* window_area) = 0;
    virtual void draw_window_collapsed(window* _window) = 0;
	virtual void draw_border();
	virtual void draw_border2(ImColor color, ImVec2 from, ImVec2 to, float line_width);
	virtual void draw_border2(ImColor color, ImRect border_rectangle);
	virtual float get_border_width();
	virtual void make_space(window* new_window) = 0;
};


class dock_vertical_painter: public dock_painter
{
public:
	dock_vertical_painter(docker* _dock) : dock_painter(_dock) {};
	virtual void adjust();
	virtual float get_inner_width();
	virtual float get_inner_height();
	virtual void make_space(window* new_window);
    virtual void draw_window_collapsed(window* _window);
};


class dock_left_painter : public dock_vertical_painter
{
public:
	dock_left_painter(docker* _dock) : dock_vertical_painter(_dock) {};
	virtual void adjust(ImRect* client_window);
	virtual ImRect get_tabbar_rectangle();
	virtual ImRect get_border_rectangle();
	virtual ImRect get_inner_rectangle();
	virtual void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position);
};


class dock_right_painter : public dock_vertical_painter
{
public:
	dock_right_painter(docker* _dock) : dock_vertical_painter(_dock) {};
	virtual void adjust(ImRect* client_window);
	virtual ImRect get_tabbar_rectangle();
	virtual ImRect get_border_rectangle();
	virtual ImRect get_inner_rectangle();
	virtual void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position);
};


class dock_horizontal_painter: public dock_painter
{
public:
	dock_horizontal_painter(docker* _dock) : dock_painter(_dock) {};
	virtual void adjust();
	virtual float get_inner_width();
	virtual float get_inner_height();
	virtual void make_space(window* new_window);
    virtual void draw_window_collapsed(window* _window);
};


class dock_bottom_painter : public dock_horizontal_painter
{
public:
	dock_bottom_painter(docker* _dock) : dock_horizontal_painter(_dock) {};
	virtual void adjust(ImRect* client_window);
	virtual ImRect get_tabbar_rectangle();
	virtual ImRect get_border_rectangle();
	virtual ImRect get_inner_rectangle();
	virtual void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position);
};

class dock_top_painter : public dock_horizontal_painter
{
public:
	dock_top_painter(docker* _dock) : dock_horizontal_painter(_dock) {};
	virtual void adjust(ImRect* client_window);
	virtual ImRect get_tabbar_rectangle();
	virtual ImRect get_border_rectangle();
	virtual ImRect get_inner_rectangle();
	virtual void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position);
};

class dock_fill_painter : public dock_painter
{
public:
	dock_fill_painter(docker* _dock) : dock_painter(_dock) {};
	virtual ImRect get_border_rectangle();
	virtual ImRect get_inner_rectangle();
	virtual ImRect get_tabbar_rectangle();
	virtual void resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position);
	virtual void adjust();
	virtual void adjust(ImRect* client_window);
	virtual float get_inner_width();
	virtual float get_inner_height();
	virtual void make_space(window* new_window);
    virtual void draw_window_collapsed(window* _window);
};

}

#endif
