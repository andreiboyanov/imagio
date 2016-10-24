#include "dock.h"

#include <iostream>


namespace wimgui
{

docker::docker(const char* title) : background_window(title)
{
	this->style = dock_left;
}

docker::docker(const char* title, dock_style style) : background_window(title)
{
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

float docker::get_inner_width()
{
	return this->get_width() - window_extra_area;
}

float docker::get_inner_height()
{
	return this->get_height() - window_extra_area;
}

void docker::draw_imgui()
{
	background_window::draw_imgui();
	for (auto window : this->windows)
	{
		window->draw_imgui();
	}
}

void docker::draw()
{
	ImGuiContext& context = *GImGui;
	const ImGuiIO& io = context.IO;
	if (this->get_inner_width() - hoover_delta <= io.MousePos.x &&
		io.MousePos.x <= this->get_inner_width() + hoover_delta)
	{
		this->draw(draw_resizing);
	}
	else
	{
		this->draw(draw_normal);
	}
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
		this->draw_border(ImColor(1.0f, 0.0f, 0.0f, 0.3f), (float)window_extra_area);
		break;
	}
	case draw_none:
	{
		break;
	}

	}
}

void docker::adjust(window_area* client_window)
{
	this->set_position(client_window->left, client_window->top);
	this->set_width(client_window->width + window_extra_area);
	this->set_height(client_window->height);
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
		if (window_width != this->get_inner_width()) {
			this->set_width(window_width + window_extra_area);
			break;
		}
	}
	float last_y = this->get_position().y;
	for (auto window : this->windows)
	{
		window->set_width(this->get_inner_width());
		window->set_position(this->get_position().x, last_y);

		float window_height = window->get_current_height();
		if (!window->is_collapsed())
			window->set_height(window_height);

		last_y += window_height;
	}
	this->set_height(last_y + window_extra_area - this->get_position().y);
}


#pragma region protected methods

void docker::draw_border(ImColor color, float line_width)
{
	ImGuiWindow* window = this->get_imgui_window();
	if (window)
	{
		float width = this->get_inner_width();
		ImVec2 from(width + (line_width / 2), this->get_position().y);
		ImVec2 to(width + (line_width / 2), this->get_inner_height() + this->get_position().y);
		window->DrawList->AddLine(from, to, color, line_width);

		from.x = 0; from.y = to.y + (line_width / 2);
		to.x = width + line_width; to.y += (line_width / 2);
		window->DrawList->AddLine(from, to, color, line_width);
	}
}

#pragma endregion

}