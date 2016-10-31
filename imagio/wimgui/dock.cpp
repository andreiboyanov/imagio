#include "dock.h"

#include <iostream>


namespace wimgui
{

docker::docker(const char* _title) : background_window(_title)
{
	this->set_dock_style(dock_left);
	this->set_min_width();
	this->set_min_height();
}

docker::docker(const char* _title, dock_style _style) : background_window(_title)
{
	this->set_dock_style(_style);
	this->set_min_width();
	this->set_min_height();
}

docker::~docker()
{

}

void docker::set_dock_style(dock_style _style)
{
	this->style = _style;
	if (this->painter)
		delete this->painter;
	switch (style)
	{
	case dock_left:
		this->painter = new dock_left_painter(this);
		break;
    case dock_top:
        break;
    case dock_bottom:
		this->painter = new dock_bottom_painter(this);
        break;
    case dock_fill:
        break;
    case dock_right:
        break;
	}
}

void docker::add_window(window *_window)
{
	windows.push_back(_window);
	_window->dock_to(this);
	this->painter->adjust();
}

void docker::remove_window(window *_window)
{
	_window->dock_to(nullptr);
}

float docker::get_inner_width()
{
	return this->painter->get_inner_width();
}

float docker::get_inner_height()
{
	return this->painter->get_inner_height();
}

void docker::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	this->painter->resize(mouse_position, mouse_clicked_position);
}

void docker::draw_imgui()
{
	background_window::draw_imgui();
	for (auto _window : this->windows)
	{
		_window->draw_imgui();
	}
}

void docker::draw()
{
	ImGuiContext& context = *GImGui;
	bool hovered, held;
	const ImRect border = painter->get_border_rectangle();
	ImGui::ButtonBehavior(border,
		  					this->get_imgui_window()->GetID("#RESIZE"),
							&hovered, &held,
							ImGuiButtonFlags_FlattenChilds);

	if (hovered)
	{
		context.MouseCursor = ImGuiMouseCursor_ResizeNWSE;
		this->draw(draw_hovered);
	}
	else if (held)
	{
		context.MouseCursor = ImGuiMouseCursor_ResizeNWSE;
		this->draw(draw_resizing);
		this->resize(context.IO.MousePos, context.ActiveIdClickOffset);
	}
	else
	{
		this->draw(draw_normal);
	}
}

void docker::draw(dock_draw_mode mode)
{
	this->painter->draw_border(mode);
}

void docker::adjust(window_area* client_window)
{
	this->painter->adjust(client_window);
}


#pragma region protected methods

void docker::set_min_width()
{
	if (this->get_width() < 100.0f)
		this->set_width(250.0f);
}

void docker::set_min_height()
{
	if (this->get_height() < 100.0f)
		this->set_height(250.0f);
}

#pragma endregion



dock_painter::dock_painter(docker* _dock)
{
	this->dock = _dock;
}

dock_painter::~dock_painter()
{

}

void dock_painter::draw_border2(ImColor color, ImVec2 from, ImVec2 to, float line_width)
{
	ImGuiWindow* imgui_window = this->dock->get_imgui_window();
	if (imgui_window)
	{
		imgui_window->DrawList->AddLine(from, to, color, line_width);
	}
}

void dock_painter::draw_border2(ImColor color, ImRect border_rectangle)
{
	ImGuiWindow* imgui_window = this->dock->get_imgui_window();
	if (imgui_window)
	{
		imgui_window->DrawList->AddRectFilled(border_rectangle.Min, border_rectangle.Max, color);
	}
}


void dock_left_painter::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	this->dock->set_width(mouse_position.x +
							(hover_delta - mouse_clicked_position.x) +
							window_extra_area);
}

ImRect dock_left_painter::get_border_rectangle()
{
	ImVec2 position = dock->get_position();
	ImVec2 size = dock->get_size();
	return ImRect(position.x + size.x - window_extra_area,
				  position.y,
				  position.x + size.x - window_extra_area + hover_delta,
				  position.y + size.y);
}

void dock_left_painter::draw_border(dock_draw_mode mode)
{
	switch (mode)
	{
	case draw_normal:
	{
		float line_width = 2.0f;
		float width = this->get_inner_width();
		ImVec2 position = this->dock->get_position();
		this->draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGrip),
							ImVec2(width + (line_width / 2), position.y),
							ImVec2(width + (line_width / 2),
									this->get_inner_height() + position.y),
							line_width);
		break;
	}
	case draw_hovered:
	{
		this->draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripHovered), this->get_border_rectangle());
		break;
	}
	case draw_resizing:
	{
		this->draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripActive), this->get_border_rectangle());
		break;
	}
	case draw_none:
	{
		break;
	}

	}
}

void dock_left_painter::adjust(window_area* client_window)
{
	dock->set_position(client_window->left, client_window->top);
	dock->set_height(client_window->height);
	this->adjust();
}

void dock_left_painter::adjust()
{
	if (this->sleep > 0) {
		this->sleep--;
		return;
	}
	if (!dock->windows.size())
		return;
	float last_y = dock->get_position().y;
	for (auto window : dock->windows)
	{
		window->set_width(dock->get_inner_width());
		window->set_position(dock->get_position().x, last_y);

		float window_height = window->get_current_height();
		if (!window->is_collapsed())
			window->set_height(window_height);

		last_y += window_height;
	}
}

float dock_left_painter::get_inner_width()
{
	return dock->get_width() - window_extra_area;
}

float dock_left_painter::get_inner_height()
{
	return dock->get_height();
}

void dock_bottom_painter::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	float parent_height = this->dock->position.y + this->dock->size.y;
	this->dock->position.y = mouse_position.y + mouse_clicked_position.y -
								window_extra_area;
	this->dock->size.y = parent_height - this->dock->position.y;
}

ImRect dock_bottom_painter::get_border_rectangle()
{
	ImVec2 position = dock->get_position();
	ImVec2 size = dock->get_size();
	return ImRect(position.x,
				  position.y + window_extra_area - hover_delta,
				  position.x + size.x,
				  position.y + window_extra_area);
}

// FIXME: This method should be in the dock_painter class.
void dock_bottom_painter::draw_border(dock_draw_mode mode)
{
	switch (mode)
	{
	case draw_normal:
	{
		float line_width = 2.0f;
		ImVec2 position = this->dock->get_position();
		float width = this->get_inner_width();
		float y = position.y + window_extra_area - line_width / 2;
		this->draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGrip),
							ImVec2(position.x, y),
							ImVec2(position.x + width, y),
							line_width);
		break;
	}
	case draw_hovered:
	{
		this->draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripHovered), this->get_border_rectangle());
		break;
	}
	case draw_resizing:
	{
		this->draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripActive), this->get_border_rectangle());
		break;
	}
	case draw_none:
	{
		break;
	}

	}
}

void dock_bottom_painter::adjust(window_area* client_window)
{
	dock->set_position(client_window->left, client_window->top + client_window->height
						- this->dock->get_height());
	dock->set_width(client_window->width);
	this->adjust();
}

void dock_bottom_painter::adjust()
{
	if (this->sleep > 0) {
		this->sleep--;
		return;
	}
	if (!dock->windows.size())
		return;
	float last_x = dock->get_position().x;
	for (auto window : dock->windows)
	{
		window->set_height(dock->get_inner_height());
		window->set_position(last_x, dock->get_position().y + window_extra_area);

		float window_width = window->get_current_width();
		if (!window->is_collapsed())
			window->set_width(window_width);

		last_x += window_width;
	}
}

float dock_bottom_painter::get_inner_width()
{
	return dock->get_width();
}

float dock_bottom_painter::get_inner_height()
{
	return dock->get_height() - window_extra_area;
}
}
