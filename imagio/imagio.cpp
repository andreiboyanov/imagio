#include <iostream>

#include "imgui/imgui.h"
#include "imagio.h"

#include "wimgui/workspace.h"
#include "wimgui/window.h"
#include "wimgui/dock.h"
#include "wimgui/file_explorer.h"
#include "wimgui/3dpaint.h"


namespace imagio {


class window_one : public wimgui::window {
private:
	wimgui::painter3d* painter;

public:
	window_one(const char* _title) : window(_title)
	{
		painter = new wimgui::painter3d(this);
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
		painter->draw_zero_cross();
		painter->draw_axes();
		for (float x = 0.0f, y = 0.0f, z = 0.0f;
			x <= 1000.0f; x += 5.0f, y += 5.0f, z += 5.0f)
		{
			painter->draw_point(Vector3f(x, y, z), ImColor(x / 1000.0f, y / 1000.0f, z / 1000.0f));
		}
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
// wimgui::file_explorer explorer = wimgui::file_explorer("/", "Choose file");
wimgui::docker dock_left("##DOCK LEFT", wimgui::dock_left);
wimgui::docker dock_bottom("##DOCK BOTTOM", wimgui::dock_bottom);
wimgui::docker dock_top("##DOCK TOP", wimgui::dock_fill);
// wimgui::docker dock_right("##DOCK RIGHT", wimgui::dock_right);
// wimgui::docker dock_fill("##DOCK_FILL", wimgui::dock_fill);


void init()
{
	workspace.add_dock(&dock_bottom);
	workspace.add_dock(&dock_left);
	workspace.add_dock(&dock_top);
	// workspace.add_dock(&dock_right);
	// workspace.add_dock(&dock_top);

	workspace.add_window(&window1, &dock_top);
	workspace.add_window(&window2, &dock_bottom);
	workspace.add_window(&window3, &dock_bottom);
	// workspace.add_window(&window4, &dock_right);
	workspace.add_window(&window5, &dock_left);

	// explorer.set_dockable(false);
	// explorer.allow_resize(true);
	// explorer.set_height(workspace.get_height() / 2);
	// explorer.set_width(workspace.get_width() / 2);
	// workspace.add_window(&explorer, &dock_top);
}

int draw()
{
	draw_main_menu();
	workspace.draw_workspace();
	ImGui::ShowTestWindow();
	return 0;
}

}
