#include "window.h"
#include "workspace.h"
#include "../imgui/imgui.h"

namespace wimgui
{

workspace::workspace()
{
	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
}

workspace::~workspace() {}

void workspace::add_window(wimgui::window *window)
{
	windows.push_back(window);
}

void workspace::add_dock(docker *dock)
{
	docks.push_back(dock);
}

void workspace::draw()
{
	for (auto window: windows)
	{
		window->draw_imgui();
	}
}

}