#include "workspace.h"
#include <iostream>

namespace wimgui
{

static const char* background_window_name = "##WORKSPACE_WINDOW";

#pragma region constructors

workspace::workspace() : background_window(background_window_name)
{
	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
}

workspace::workspace(const char* title) : background_window(title)
{
	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
}

workspace::~workspace() {}

#pragma endregion

#pragma region public methods

void workspace::add_window(wimgui::window *_window)
{
	windows.push_back(_window);
}


void workspace::add_dock(docker *_dock)
{
	docks.push_back(_dock);
}

// FIXME: Get size and position from ImGui
ImRect* workspace::get_client_area()
{

	ImGuiWindow* main_menu_window = ImGui::FindWindowByName("##MainMenuBar");
	ImGuiContext& context = *GImGui;

	static ImRect* client = new ImRect();
	float top = main_menu_window ? main_menu_window->Size.y : 50.0f;
	client->Min.x = 0.0f;
	client->Min.y = top;
	client->Max.x = context.IO.DisplaySize.x;
	client->Max.y =	context.IO.DisplaySize.y;
	return client;
}

void workspace::draw()
{
	ImRect* client = this->get_client_area();
	for (auto _dock : this->docks)
	{
		_dock->adjust(client);
		_dock->draw_imgui();
	}
}

void workspace::draw_workspace()
{
	ImRect* client = this->get_client_area();
	this->set_position(client->Min.x, client->Min.y);
	this->set_width(client->GetWidth());
	this->set_height(client->GetHeight());

	this->draw();

	for (auto _window: this->windows)
	{
		if (!_window->is_docked())
			_window->draw_imgui();
	}
}

#pragma endregion

#pragma region private methods


#pragma endregion

}
