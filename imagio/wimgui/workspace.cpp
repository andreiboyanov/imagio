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

workspace::workspace(const char* _title) : background_window(_title)
{
	// ImGuiStyle &style = ImGui::GetStyle();
	// style.WindowRounding = 0.0f;
}

workspace::~workspace() {}

#pragma endregion

#pragma region public methods

void workspace::add_window(window *_window)
{
	windows.push_back(_window);
}

void workspace::add_window(window *_window, docker *_dock)
{
	windows.push_back(_window);
	_dock->add_window(_window);
	// if (std::find(docks.begin(), docks.end(), _dock) == std::end(docks))
    // FIXME: fidn why std::find is not compiling
    for(auto current_dock : docks)
	{
        if(current_dock == _dock) return;
	}
    add_dock(_dock);
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
	client->Max.y = context.IO.DisplaySize.y;
	return client;
}

void workspace::draw()
{
	ImRect* client = get_client_area();
	for (auto _dock : docks)
	{
		_dock->adjust(client);
		_dock->draw_imgui();
	}
}

void workspace::draw_workspace()
{
	ImRect* client = get_client_area();
	set_position(client->Min.x, client->Min.y);
	set_width(client->GetWidth());
	set_height(client->GetHeight());

	draw();

	for (auto _window : windows)
	{
		if (!check_undocking(_window))
			check_docking(_window);
		//_window->draw_imgui();
	}
}

bool workspace::check_undocking(window* _window)
{
	bool result = false;
	ImGuiWindow* imgui_window = _window->get_imgui_window();
	
	for (auto& _dock : docks)
	{
		if (!_dock->contains(_window))
			continue;
		result = true;

		ImGuiWindow* imgui_dock = _dock->get_imgui_window();
		if (imgui_dock && imgui_window && _window->is_moving())
		{
			ImGuiContext& context = *GImGui;
			ImVec2 mouse_position = context.IO.MousePos;
			if (!(imgui_dock->Rect().Contains(mouse_position)))
			{
				_dock->remove_window(_window);
				ImVec2 window_position = _window->get_position();
				_window->set_size((mouse_position.x - window_position.x) * 2,
					(mouse_position.y - window_position.y) * 2);
			}
		}
	}
	return result;
}

bool workspace::check_docking(window* _window)
{
	bool result = false;
	if (_window->is_moving())
	{
		for (auto _dock : docks)
		{
			ImGuiWindow* imgui_dock = _dock->get_imgui_window();
			ImGuiWindow* imgui_window = _window->get_imgui_window();

			if (imgui_dock && imgui_window && _window->is_moving())
			{
				ImGuiContext& context = *GImGui;
				if (imgui_dock->Rect().Contains(context.IO.MousePos))
				{
					_dock->add_window(_window);
					result = true;
					break;
				}
			}

		}
	}
	return result;
}

#pragma endregion

#pragma region private methods


#pragma endregion

}
