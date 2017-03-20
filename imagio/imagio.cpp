#include <iostream>

#include "imgui/imgui.h"
#include "imagio.h"

#include "wimgui/workspace.h"
#include "wimgui/window.h"
#include "wimgui/dock.h"
#include "wimgui/file_explorer.h"
#include "wimgui/3dpaint.h"
#include "point_cloud_window.h"
#include "wimgui/paint_window.h"
// #include <json/json.h>

namespace imagio {


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
private:
	point_cloud_window* paint;
public:
	window_five(const char *_title, point_cloud_window* _painter) : window(_title)
	{
		paint = _painter;
		show_title(true);
		show_border(true);
		allow_resize(true);
		allow_move(true);
		show_scrollbar(false);
		allow_collapse(true);
		show_menu(false);
	}

	void draw() {
		if (ImGui::Button("Open point cloud"))
			paint->open_skv_depth("D:/src/imagio.git/imagio/sample_data/juan_00.skv");
		if (ImGui::Button("Center"))
			paint->center();
		if (ImGui::Button("View top"))
			paint->view_top();
		if (ImGui::Button("View bottom"))
			paint->view_bottom();
		if (ImGui::Button("View left"))
			paint->view_left();
		if (ImGui::Button("View right"))
			paint->view_right();
		if (ImGui::Button("View front"))
			paint->view_front();
		if (ImGui::Button("View back"))
			paint->view_back();
	}

	void load_point_cloud()
	{
	 // Json::Value point_root;
	 // std::ifstream input_file("d:/src/imagio.git/imagio/sample_data/Tagging_Robert_Juan_00.json");
	 // if (input_file.is_open())
	 // {
	 // 	input_file >> point_root;
	 // 	std::cout << point_root["ReferenceData"]["Frame"][0];
	 // }
	 // else
	 // {
	 // 	std::cout << "Problem with opening the file";
	 // }
	}
};

static wimgui::workspace workspace;

point_cloud_window window1("Point cloud");
window_two window2("Second window");
window_three window3("ImGui Metrics");
window_four window4("Fourth window");
window_five window5("Control panel", &window1);
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
	window1.get_painter()->init_view();
}

int draw()
{
	draw_main_menu();
	workspace.draw_workspace();
	ImGui::ShowTestWindow();
	return 0;
}

void custom_render()
{
	
}

}
