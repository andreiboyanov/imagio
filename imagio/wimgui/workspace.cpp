#include "workspace.h"
#include <iostream>

namespace wimgui
{

#pragma region constructors

workspace::workspace() : background_window(background_window_name)
{
	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
}

workspace::~workspace() {}

#pragma endregion

#pragma region public methods

void workspace::add_window(wimgui::window *window)
{
	windows.push_back(window);
}


void workspace::add_dock(docker *dock)
{
	docks.push_back(dock);
}

// FIXME: Get size and position from ImGui
window_area* workspace::get_client_area()
{
	static window_area client;

	ImGuiWindow* main_menu_window = ImGui::FindWindowByName("##MainMenuBar");
	ImGuiContext& context = *GImGui;

	client.top = main_menu_window ? main_menu_window->Size.y : 50.0f;
	client.left = 0.0f;
	client.width = context.IO.DisplaySize.x;
	client.height = context.IO.DisplaySize.y - client.top;
	return &client;
}

void workspace::draw()
{
	for (auto dock : this->docks)
	{
		window_area* client = this->get_client_area();
		dock->adjust(client);
		dock->draw_imgui();
	}
}

void workspace::draw_workspace()
{
	window_area* client = this->get_client_area();
	this->set_position(client->left, client->top);
	this->set_width(client->width);
	this->set_height(client->height);

	this->draw();

	for (auto window: this->windows)
	{
		if (!window->is_docked())
			window->draw_imgui();
	}
}

#pragma endregion

#pragma region private methods


#pragma endregion

}