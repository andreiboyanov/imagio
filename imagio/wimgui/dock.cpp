#include "dock.h"

#include <iostream>


namespace wimgui
{

docker::docker(const char* _title) : background_window(_title)
{
	this->set_dock_style(dock_left);
	this->set_min_width();
}

docker::docker(const char* _title, dock_style _style) : background_window(_title)
{
	this->set_dock_style(_style);
	this->set_min_width();
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
    case dock_up:
        break;
    case dock_down:
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
	return this->get_width() - window_extra_area;
}

float docker::get_inner_height()
{
	return this->get_height();
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
		this->set_width(context.IO.MousePos.x +
					    (hover_delta - context.ActiveIdClickOffset.x) +
						window_extra_area);
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
		this->draw_border(ImGui::GetColorU32(ImGuiCol_ResizeGrip));
		break;
	}
	case draw_hovered:
	{
		this->draw_border(ImGui::GetColorU32(ImGuiCol_ResizeGripHovered), hover_delta);
		break;
	}
	case draw_resizing:
	{
		this->draw_border(ImGui::GetColorU32(ImGuiCol_ResizeGripActive), hover_delta);
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
	this->painter->adjust(client_window);
}


#pragma region protected methods

void docker::draw_border(ImColor color, float line_width)
{
	ImGuiWindow* imgui_window = this->get_imgui_window();
	if (imgui_window)
	{
		float width = this->get_inner_width();
		ImVec2 from(width + (line_width / 2), this->get_position().y);
		ImVec2 to(width + (line_width / 2), this->get_inner_height() + this->get_position().y);
		imgui_window->DrawList->AddLine(from, to, color, line_width);

		from.x = 0; from.y = to.y + (line_width / 2);
		to.x = width + line_width; to.y += (line_width / 2);
		imgui_window->DrawList->AddLine(from, to, color, line_width);
	}
}

void docker::set_min_width()
{
	if (this->get_width() < 100.0f)
		this->set_width(250.0f);
}

#pragma endregion



dock_painter::dock_painter(docker* _dock)
{
	this->dock = _dock;
}

dock_painter::~dock_painter()
{

}

dock_left_painter::dock_left_painter(docker* _dock) : dock_painter(_dock)
{

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


}
