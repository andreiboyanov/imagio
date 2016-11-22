#pragma once

#ifndef WIMGUI_WINDOW_H
#define WIMGUI_WINDOW_H

#include <stddef.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

namespace wimgui {

class docker;

class window {
	bool visible = true;
	const char *title = nullptr;
	docker *dock = nullptr;
	ImGuiWindowFlags flags = 0;
	bool dockable = true;

protected:
	ImVec2 size = ImVec2(0.0f, 0.0f);
	ImVec2 position = ImVec2(0.0f, 0.0f);

public:
	window(const char* name);
	window();
	~window();

	void show_title(bool show);
	void show_border(bool show);
	void allow_resize(bool allow);
	void allow_move(bool allow);
	void show_scrollbar(bool show);
	void allow_collapse(bool allow);
	void show_menu(bool show);
	void allow_mouse_scroll(bool allow);
	void auto_resize(bool allow);
	void save_settings(bool save);
	void allow_inputs(bool allow);
	void horizontal_scrollbar(bool allow);
	void always_horizontal_scrollbar(bool always);
	void always_vertical_scrollbar(bool always);
	void focus_on_appearing(bool allow);
	void to_front_on_focus(bool allow);
	void use_window_padding(bool allow);
	void set_flags(ImGuiWindowFlags _flags);

	virtual void show(bool _visible);

	virtual void draw();
	virtual void init_draw() {}
	virtual void finish_draw() {}
	virtual void draw_imgui();

    inline const char* get_title() { return title; };
	float get_width();
	void set_width(float width);
	float get_height();
	void set_height(float height);
	ImVec2 get_size();
	void set_size(float x, float y);
	ImVec2 get_position();
	void set_position(float x, float y);
	float get_current_width();
	float get_current_height();
	bool is_collapsed();
	bool mouse_double_clicked(int button_index=0);
	void set_collapsed(bool collapsed);
	inline bool is_moving();
	inline bool is_resizing();
	void draw_vertical_text(const char* text, ImVec2 _position);

	inline docker *docked_to() { return dock; }
	inline void dock_to(docker *new_dock = nullptr)
		{ if (dockable) dock = new_dock; }
	inline bool is_docked() { return dock != nullptr; }
	inline bool is_dockable() { return dockable; }
	inline void set_dockable(bool _dockable) { dockable = _dockable;  }

	void set_cursor_position(ImVec2 _position);
	ImVec2 get_cursor_position();
	ImGuiWindow *get_imgui_window();
};


class background_window : public window
{
	ImVec4 normal_window_background;
	ImVec2 normal_window_padding;
public:
	background_window(const char* title);
	~background_window();
	virtual void init_draw();
	virtual void finish_draw();
private:
	void save_style(ImGuiStyle& style);
	void restore_style(ImGuiStyle& style);
};

}

#endif
