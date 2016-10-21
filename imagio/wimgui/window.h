#pragma once

#ifndef WIMGUI_WINDOW_H
#define WIMGUI_WINDOW_H

#include <stddef.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

namespace wimgui {

class docker;

class window {
	bool visible = true;
	const char *title = nullptr;
	ImVec2 size = ImVec2(0.0f, 0.0f);
	ImVec2 position = ImVec2(0.0f, 0.0f);
	docker *dock = nullptr;
	ImGuiWindowFlags flags = 0;

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
	virtual void draw_imgui();

	float get_preferred_width();
	void set_preferred_width(float width = -1.0f);
	float get_preferred_height();
	void set_preferred_height(float height = -1.0f);
	ImVec2 get_preferred_position();
	void set_preferred_position(float x = -1.0f, float y = -1.0f);
	float get_current_width();
	float get_current_height();
	bool is_collapsed();

	docker *docked_to();
	void dock_to(docker *new_dock = nullptr);
	bool is_docked();

	ImGuiWindow *get_imgui_window();
};

}

#endif
