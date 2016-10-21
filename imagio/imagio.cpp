#include "imgui/imgui.h"
#include "imagio.h"

#include "wimgui/workspace.h"
#include "wimgui/window.h"
#include "wimgui/dock.h"

namespace imagio {


class window_one : public wimgui::window {
public:
	window_one(char *title) : window(title)
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
	window_two(char *title) : window(title) 
	{
		show_title(false);
		show_border(false);
		allow_resize(true);
		allow_move(false);
		show_scrollbar(false);
		allow_collapse(false);
		show_menu(false);
	}

	void draw() {
		ImGui::Text("Hello, second world!");
	}
};

static wimgui::workspace workspace;
window_one window1("First window");
window_two window2("Second window");
wimgui::docker dock_left((wimgui::window *) &workspace, wimgui::dock_left);


void init()
{
	workspace.add_dock(&dock_left);
	dock_left.add_window(&window1);
	dock_left.add_window(&window2);
	workspace.add_window(&window1);
	workspace.add_window(&window2);
}

int draw()
{
	draw_main_menu();
	workspace.draw_workspace();
	return 0;
}

}