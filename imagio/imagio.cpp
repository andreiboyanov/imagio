#include <iostream>

#include "imgui/imgui.h"
#include "imagio.h"

#include "wimgui/workspace.h"
#include "wimgui/window.h"
#include "wimgui/dock.h"
#include "point_cloud/point_cloud_window.h"

namespace imagio {


class window_one : public wimgui::window {
public:
	window_one(const char *_title) : window(_title)
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
		if (is_moving())
		{
			ImGui::Text("Moving second window");
		}
	}
};

class window_two : public window_one
{
public:
	window_two(const char * _title) : window_one(_title) {}
	void draw()
	{
		ImGui::Text("Window 2 content");
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
	point_cloud_window* point_cloud;
public:
	window_five(const char *_title, point_cloud_window* _point_cloud) : window(_title), point_cloud(_point_cloud)
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
		if(ImGui::Button(" << "))
		{
			point_cloud->move_backward();
		}
        ImGui::SameLine();
		if(ImGui::Button(" >> "))
		{
			point_cloud->move_forward();
		}
		if(ImGui::Button(" Top "))
		{
			point_cloud->view_top();
		}
        ImGui::SameLine();
		if(ImGui::Button(" Bottom "))
		{
			point_cloud->view_bottom();
		}
		if(ImGui::Button(" Left "))
		{
			point_cloud->view_left();
		}
        ImGui::SameLine();
		if(ImGui::Button(" Right "))
		{
			point_cloud->view_right();
		}
		if(ImGui::Button(" Front "))
		{
			point_cloud->view_front();
		}
        ImGui::SameLine();
		if(ImGui::Button(" Back "))
		{
			point_cloud->view_back();
		}

		ImGui::Text("Frame %5d / %5d", point_cloud->get_current_frame(), point_cloud->get_frames_count() - 1);
	}

};

static std::vector<wimgui::workspace> workspaces =
{
	wimgui::workspace("Layout 1"),
	wimgui::workspace("Layout 2")
};
static wimgui::workspace* active_workspace = &workspaces[0];

point_cloud_window *point_cloud_viewer;
window_two window2("Second window");
window_three window3("ImGui Metrics");
window_four window4("Fourth window");
window_five* window5;

void init()
{
	point_cloud_viewer = new point_cloud_window("Point cloud");
	window5 = new window_five("Control panel", point_cloud_viewer);
	workspaces[0].add_window(point_cloud_viewer);
	workspaces[0].add_window(&window2);
	workspaces[0].add_window(&window3);
	workspaces[0].add_window(&window4);
	workspaces[0].add_window(window5);

	workspaces[1].add_window(window5);
	workspaces[1].add_window(point_cloud_viewer);

}

int draw()
{
	draw_main_menu();
	active_workspace->draw_workspace();
	return 0;
}

void draw_main_menu() {
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("(dummy menu)", NULL, false, false);
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Hello");
					ImGui::MenuItem("Sailor");
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}
			ImGui::Separator();
			if (ImGui::BeginMenu("Options"))
			{
				static bool enabled = true;
				ImGui::MenuItem("Enabled", "", &enabled);
				ImGui::BeginChild("child", ImVec2(0, 60), true);
				for (int i = 0; i < 10; i++)
					ImGui::Text("Scrolling Text %d", i);
				ImGui::EndChild();
				static float f = 0.5f;
				static int n = 0;
				ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
				ImGui::InputFloat("Input", &f, 0.1f);
				ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Colors"))
			{
				for (int i = 0; i < ImGuiCol_COUNT; i++)
					ImGui::MenuItem(ImGui::GetStyleColorName((ImGuiCol)i));
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Disabled", false)) // Disabled
			{
				IM_ASSERT(0);
			}
			if (ImGui::MenuItem("Checked", NULL, true)) {}
			if (ImGui::MenuItem("Quit", "Alt+F4")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Workspace"))
		{
			for (auto& workspace : workspaces)
			{
				if (ImGui::MenuItem(workspace.get_title()))
				{
					active_workspace = &workspace;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

}
