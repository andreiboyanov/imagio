#include <iostream>

#include "imgui/imgui.h"
#include "imagio.h"

#include "wimgui/workspace.h"
#include "wimgui/window.h"
#include "wimgui/dock.h"
#include "wimgui/file_explorer.h"


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

	void draw()
	{
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
		allow_move(true);
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

class window_four : public wimgui::window {
public:
	window_four(const char *_title) : window(_title)
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
		ImGui::Text("Hello, fourth world!");
	}
};

class window_five : public wimgui::window {
public:
	window_five(const char *_title) : window(_title)
	{
		show_title(true);
		show_border(true);
		allow_resize(true);
		allow_move(true);
		show_scrollbar(false);
		allow_collapse(true);
		show_menu(false);
	}

	void draw() {
		ImGui::Text("Hello, fifth world!");
	}
};

static wimgui::workspace workspace;
window_one window1("First window");
window_two window2("Second window");
window_three window3("ImGui Metrics");
window_four window4("Fourth window");
window_five window5("Fifth window");
wimgui::file_explorer explorer = wimgui::file_explorer("/", "Choose file");
wimgui::docker dock_left("##DOCK LEFT", wimgui::dock_left);
wimgui::docker dock_bottom("##DOCK BOTTOM", wimgui::dock_bottom);
wimgui::docker dock_top("##DOCK TOP", wimgui::dock_top);
wimgui::docker dock_right("##DOCK RIGHT", wimgui::dock_right);
wimgui::docker dock_fill("##DOCK_FILL", wimgui::dock_top);


void init()
{
	workspace.add_dock(&dock_bottom);
	workspace.add_dock(&dock_left);
	workspace.add_dock(&dock_top);
	workspace.add_dock(&dock_right);
	workspace.add_dock(&dock_fill);

	workspace.add_window(&window1, &dock_left);
	workspace.add_window(&window2, &dock_bottom);
	workspace.add_window(&window3, &dock_bottom);
	workspace.add_window(&window4, &dock_right);
	workspace.add_window(&window5, &dock_left);
	workspace.add_window(&explorer, &dock_top);
}

int draw()
{
	draw_main_menu();
	workspace.draw_workspace();
	ImGui::ShowTestWindow();
	return 0;
}

}
