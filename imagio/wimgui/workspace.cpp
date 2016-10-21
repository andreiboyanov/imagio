#include "workspace.h"
#include <iostream>

namespace wimgui
{

#pragma region constructors

workspace::workspace() : window(background_window_name)
{
	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	this->set_background_window();
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

	client.top = main_menu_window ? main_menu_window->Size.y : 50.0f;
	client.left = 0.0f;
	client.width = 800.0f;
	client.height = 600.0f;
	return &client;
}

void workspace::draw()
{
	for (auto dock : this->docks)
	{
		window_area* client = this->get_client_area();
		dock->adjust(client);
		dock->draw();
	}
}

void workspace::draw_workspace()
{
	window_area* client = this->get_client_area();
	this->set_preferred_position(client->left, client->top);
	this->set_preferred_width(client->width);
	this->set_preferred_height(client->height);

	ImVec4 color = ImColor(114, 144, 154);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, color);
	this->draw_imgui();
	ImGui::PopStyleColor();

	for (auto window: this->windows)
	{
		if (!window->is_docked())
			window->draw_imgui();
	}

	ImGui::ShowMetricsWindow();
}

#pragma endregion

#pragma region private methods

void workspace::set_background_window()
{
	this->allow_collapse(false);
	this->allow_inputs(false);
	this->allow_mouse_scroll(false);
	this->allow_resize(false);
	this->allow_move(false);
	this->always_horizontal_scrollbar(false);
	this->always_vertical_scrollbar(false);
	this->save_settings(false);
	this->auto_resize(false);
	this->focus_on_appearing(false);
	this->horizontal_scrollbar(false);
	this->show(true);
	this->show_border(false);
	this->show_menu(false);
	this->show_scrollbar(false);
	this->show_title(false);
	this->to_front_on_focus(false);
	this->use_window_padding(false);

}

#pragma endregion

}