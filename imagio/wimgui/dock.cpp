#include "dock.h"

#include <iostream>


namespace wimgui
{

docker::docker(window* parent_window)
{
	this->parent = parent_window;
	this->style = dock_left;
}

docker::docker(window* parent_window, dock_style style)
{
	this->parent = parent_window;
	this->style = style;
}

docker::~docker()
{

}

void docker::add_window(window *window)
{
	windows.push_back(window);
	window->dock_to(this);
	this->adjust();
}

void docker::remove_window(window *window)
{
	window;
}

ImVec2 docker::preferred_size(float width, float height)
{
	if (width > 0.0f)
		this->size.x = width;
	if (height > 0.0f)
		this->size.y = height;
	return this->size;
}

ImVec2 docker::preferred_position(float x, float y)
{
	if (x >= 0.0f)
		this->position.x = x;
	if (y >= 0.0f)
		this->position.y = y;
	return this->position;
}

void docker::draw(dock_draw_mode mode)
{
	switch (mode)
	{
	case draw_normal:
	{
		this->draw_border(ImGui::GetColorU32(ImGuiCol_Text));
		break;
	}
	case draw_resizing:
	{
		this->draw_border(ImColor(255, 0, 0));
		break;
	}
	case draw_none:
	{
		break;
	}

	}
}

void docker::draw()
{
	ImGuiContext& context = *GImGui;
	const ImGuiIO& io = context.IO;
	if (this->size.x - hoover_delta <= io.MousePos.x &&
		io.MousePos.x <= this->size.x + hoover_delta)
	{
		this->draw(draw_resizing);
	}
	else
	{
		this->draw(draw_normal);
	}
	for (auto window : this->windows)
	{
		window->draw_imgui();
	}
}

void docker::adjust(window_area* client_window)
{
	this->position.x = client_window->left;
	this->position.y = client_window->top;
	this->size.x = client_window->width;
	this->size.y = client_window->height;
	this->adjust();
}

void docker::adjust()
{
	if (this->sleep > 0) {
		this->sleep--;
		return;
	}
	if (!this->windows.size())
		return;
	for (auto window : this->windows)
	{
		float window_width = window->get_current_width();
		if (window_width != this->size.x) {
			this->size.x = window_width;
			break;
		}
	}
	float last_y = this->position.y;
	for (auto window : this->windows)
	{
		window->set_preferred_width(this->size.x);
		window->set_preferred_position(this->position.x, last_y);

		float window_height = window->get_current_height();
		if (!window->is_collapsed())
			window->set_preferred_height(window_height);

		last_y += window_height;
	}
	this->size.y = last_y;
}


#pragma region protected methods

void docker::draw_border(ImColor color)
{
	ImGuiWindow* window = parent->get_imgui_window();
	if (window)
	{
		ImVec2 from(this->size.x, this->position.y);
		ImVec2 to(this->size.x, this->size.y);
		window->DrawList->AddLine(from, to, color);

		from.x = 0; from.y = this->size.y;
		to.x = this->size.x; to.y = this->size.y;
		window->DrawList->AddLine(from, to, color);
	}
}

#pragma endregion

}