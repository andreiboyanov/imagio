#include <iostream>

#include "imgui/imgui.h"
#include "imagio.h"

#include "wimgui/workspace.h"
#include "wimgui/window.h"
#include "wimgui/dock.h"

namespace imagio {


class window_one : public wimgui::window {
public:
	window_one(const char* _title) : window(_title)
	{
		show_title(true);
		show_border(true);
		allow_resize(true);
		allow_move(true);
		show_scrollbar(true);
		allow_collapse(true);
		show_menu(false);
	}

	void draw() {
		ImGui::Text("Hello, world!");
	}
};


class window_two : public wimgui::window {
public:
	window_two(const char *_title) : window(_title)
	{
		show_title(true);
		show_border(true);
		allow_resize(true);
		allow_move(false);
		show_scrollbar(false);
		allow_collapse(true);
		show_menu(false);
	}

	void draw() {
		ImGui::Text("Hello, second world!");
	}
};

class window_three : public wimgui::window {
public:
	window_three(const char *_title) : window(_title)
	{
		show_border(true);
	}

	void draw() {
		ImGui::ShowMetricsWindow();
	}
};

static wimgui::workspace workspace;
window_one window1("First window");
window_two window2("Second window");
window_three window3("ImGui Metrics");
wimgui::docker dock_left("##DOCK LEFT", wimgui::dock_left);
wimgui::docker dock_bottom("##DOCK BOTTOM", wimgui::dock_bottom);


void init()
{
	workspace.add_dock(&dock_left);
	workspace.add_dock(&dock_bottom);
	dock_left.add_window(&window1);
	dock_left.add_window(&window3);
	dock_bottom.add_window(&window2);
	workspace.add_window(&window1);
	workspace.add_window(&window2);
	workspace.add_window(&window3);
}

int draw()
{
	draw_main_menu();
	workspace.draw_workspace();
	return 0;
}

}
