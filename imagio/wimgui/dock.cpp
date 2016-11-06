#include "dock.h"

#include <iostream>


namespace wimgui
{

docker::docker(const char* _title) : background_window(_title)
{
	set_dock_style(dock_left);
	set_min_width();
	set_min_height();
}

docker::docker(const char* _title, dock_style _style) : background_window(_title)
{
	set_dock_style(_style);
	set_min_width();
	set_min_height();
}

docker::~docker()
{

}

void docker::set_dock_style(dock_style _style)
{
	style = _style;
	if (painter)
		delete painter;
	switch (style)
	{
	case dock_left:
		painter = new dock_left_painter(this);
		break;
	case dock_top:
		painter = new dock_top_painter(this);
		break;
	case dock_bottom:
		painter = new dock_bottom_painter(this);
		break;
	case dock_fill:
		painter = new dock_fill_painter(this);
		break;
	case dock_right:
		painter = new dock_right_painter(this);
		break;
	}
}

void docker::add_window(window *_window)
{
	painter->make_space(_window);
	windows.push_back(_window);
	_window->dock_to(this);
}

void docker::fill_free_space(bool _fill)
{
	fill = _fill;
}

void docker::remove_window(window *_window)
{
	_window->dock_to(nullptr);
	windows.remove(_window);
}

float docker::get_inner_width()
{
	return painter->get_inner_width();
}

float docker::get_inner_height()
{
	return painter->get_inner_height();
}

void docker::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	painter->resize(mouse_position, mouse_clicked_position);
}

void docker::draw_imgui()
{
	background_window::draw_imgui();
	for (auto _window : windows)
	{
        if(_window->is_collapsed())
            painter->draw_window_collapsed(_window);
        else
		    _window->draw_imgui();
	}
}

void docker::draw()
{
	ImGuiContext& context = *GImGui;
	const ImRect border = painter->get_border_rectangle(draw_hovered);
	ImGui::ButtonBehavior(border,
		get_imgui_window()->GetID("#RESIZE"),
		&(border_hovered), &(border_held),
		ImGuiButtonFlags_FlattenChilds);

	if (border_hovered)
	{
		context.MouseCursor = ImGuiMouseCursor_ResizeNWSE;
		draw(draw_hovered);
	}
	else if (border_held)
	{
		context.MouseCursor = ImGuiMouseCursor_ResizeNWSE;
		draw(draw_resizing);
		resize(context.IO.MousePos, context.ActiveIdClickOffset);
	}
	else
	{
		draw(draw_normal);
	}
}

void docker::draw(dock_draw_mode mode)
{
	painter->draw_border(mode);
}

void docker::adjust(ImRect* client_window)
{
	painter->adjust(client_window);
}


#pragma region protected methods

void docker::set_min_width()
{
	if (get_width() < 100.0f)
		set_width(250.0f);
}

void docker::set_min_height()
{
	if (get_height() < 100.0f)
		set_height(250.0f);
}

#pragma endregion


#pragma region dock_painter

dock_painter::dock_painter(docker* _dock)
{
	dock = _dock;
}

dock_painter::~dock_painter()
{

}

void dock_painter::draw_border(dock_draw_mode mode)
{
	switch (mode)
	{
	case draw_normal:
	{
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGrip), get_border_rectangle(mode));
		break;
	}
	case draw_hovered:
	{
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripHovered), get_border_rectangle(mode));
		break;
	}
	case draw_resizing:
	{
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripActive), get_border_rectangle(mode));
		break;
	}
	case draw_none:
	{
		break;
	}

	}
}


void dock_painter::draw_border2(ImColor color, ImVec2 from, ImVec2 to, float line_width)
{
	ImGuiWindow* imgui_window = dock->get_imgui_window();
	if (imgui_window)
	{
		imgui_window->DrawList->AddLine(from, to, color, line_width);
	}
}

void dock_painter::draw_border2(ImColor color, ImRect border_rectangle)
{
	ImGuiWindow* imgui_window = dock->get_imgui_window();
	if (imgui_window)
	{
		imgui_window->DrawList->AddRectFilled(border_rectangle.Min, border_rectangle.Max, color);
	}
}

float dock_painter::get_border_width()
{
	return (dock->border_hovered || dock->border_held) ? hover_delta : 2.0f;
}

float dock_painter::get_border_width(dock_draw_mode mode)
{
	return (mode == draw_hovered || mode == draw_resizing) ? hover_delta : 2.0f;
}


#pragma endregion

#pragma region dock_vertical_painter

void dock_vertical_painter::adjust()
{
	if (sleep > 0) {
		sleep--;
		return;
	}
	if (!dock->windows.size())
		return;

	ImRect rectangle = get_inner_rectangle();
	float last_y = rectangle.Min.y;
	for (auto window : dock->windows)
	{
		window->set_width(rectangle.GetWidth());
		window->set_position(rectangle.Min.x, last_y);

		last_y += window->get_height();

		float window_height = window->get_current_height();
		if (!window->is_collapsed())
			window->set_height(window_height);

	}

	if (dock->fill)
	{
		window* last_window = dock->windows.back();
		last_window->set_height(rectangle.Max.y -
			last_window->get_position().y);
	}
}

float dock_vertical_painter::get_inner_width()
{
	return dock->get_width() - window_extra_area;
}

float dock_vertical_painter::get_inner_height()
{
	return dock->get_height();
}

void dock_vertical_painter::make_space(window* new_window)
{
	if (!dock->windows.size())
		return;
	float new_height = new_window->get_height();
	window* last_window = dock->windows.back();
	float last_height = last_window->get_height();
	if (last_height > new_height + 50.0f)
	{
		last_window->set_height(last_height - new_height);
	}
	else
	{
		last_window->set_height(50.0f);
		new_window->set_height(last_height - 50.0f);
	}
	sleep += 2;
}

void dock_vertical_painter::draw_window_collapsed(window* _window)
{

}

#pragma endregion


#pragma region dock_left_painter

void dock_left_painter::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	dock->set_width(mouse_position.x -
		mouse_clicked_position.x +
		window_extra_area - dock->position.x);
}

ImRect dock_left_painter::get_border_rectangle(dock_draw_mode mode)
{
	float line_width = get_border_width(mode);
	ImVec2 position = dock->position;
	ImVec2 size = dock->size;
	return ImRect(position.x + size.x - window_extra_area,
		position.y,
		position.x + size.x - window_extra_area + line_width,
		position.y + size.y);
}

ImRect dock_left_painter::get_inner_rectangle()
{
	ImVec2 position = dock->position;
	ImVec2 size = dock->size;
	return ImRect(position.x, position.y,
		position.x + size.x - window_extra_area,
		position.y + size.y);
}

void dock_left_painter::adjust(ImRect* client_window)
{
	dock->set_position(client_window->Min.x, client_window->Min.y);
	dock->set_height(client_window->GetHeight());
	dock_vertical_painter::adjust();
	client_window->Min.x = dock->get_width() +
		dock->get_position().x -
		window_extra_area + get_border_width();
}

#pragma endregion


#pragma region dock_right_painter

void dock_right_painter::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	float initial_x = dock->position.x;
	dock->position.x = mouse_position.x +
		mouse_clicked_position.x -
		window_extra_area;
	dock->size.x += initial_x - dock->position.x;
}

ImRect dock_right_painter::get_border_rectangle(dock_draw_mode mode)
{
	float line_width = get_border_width(mode);
	ImVec2 position = dock->position;
	ImVec2 size = dock->size;
	return ImRect(position.x + window_extra_area - line_width,
		position.y,
		position.x + window_extra_area,
		position.y + size.y);
}

ImRect dock_right_painter::get_inner_rectangle()
{
	ImVec2 position = dock->position;
	ImVec2 size = dock->size;
	return ImRect(position.x + window_extra_area, position.y,
		position.x + size.x,
		position.y + size.y);
}

void dock_right_painter::adjust(ImRect* client_window)
{
	dock->set_position(client_window->Max.x - dock->size.x,
					   client_window->Min.y);
	dock->set_height(client_window->GetHeight());
	dock_vertical_painter::adjust();
	client_window->Max.x = dock->position.x +
		window_extra_area - get_border_width();
}

#pragma endregion


#pragma region dock_horizontal_painter

void dock_horizontal_painter::adjust()
{
	if (sleep > 0) {
		sleep--;
		return;
	}
	if (!dock->windows.size())
		return;

	ImRect rectangle = get_inner_rectangle();
	float last_x = rectangle.Min.x;
	for (auto window : dock->windows)
	{
		window->set_height(rectangle.GetHeight());
		window->set_position(last_x, rectangle.Min.y);

		float window_width = window->get_current_width();
		if (!window->is_collapsed())
			window->set_width(window_width);

		last_x += window_width;
	}

	if (dock->fill)
	{
		window* last_window = dock->windows.back();
		last_window->set_width(rectangle.Max.x -
									last_window->get_position().x);
	}
}

float dock_horizontal_painter::get_inner_width()
{
	return dock->get_width();
}

float dock_horizontal_painter::get_inner_height()
{
	return dock->get_height() - window_extra_area;
}

void dock_horizontal_painter::make_space(window* new_window)
{
	if (!dock->windows.size())
		return;
	float new_width = new_window->get_width();
	window* last_window = dock->windows.back();
	float last_width = last_window->get_width();
	if (last_width > new_width + 50.0f)
	{
		last_window->set_width(last_width - new_width);
	}
	else
	{
		last_window->set_width(50.0f);
		new_window->set_height(last_width - 50.0f);
	}
	sleep += 2;
}

void dock_horizontal_painter::draw_window_collapsed(window* _window)
{

}

#pragma endregion


#pragma region dock_top_painter

void dock_top_painter::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	dock->size.y = mouse_position.y + mouse_clicked_position.y +
		window_extra_area - dock->position.y;
}

ImRect dock_top_painter::get_border_rectangle(dock_draw_mode mode)
{
	float line_width = get_border_width(mode);
	ImVec2 position = dock->position;
	ImVec2 size = dock->size;
	return ImRect(position.x,
		position.y + size.y - window_extra_area,
		position.x + size.x,
		position.y + size.y - window_extra_area + line_width);
}

ImRect dock_top_painter::get_inner_rectangle()
{
	ImVec2 position = dock->position;
	ImVec2 size = dock->size;
	return ImRect(position.x, position.y,
		position.x + size.x,
		position.y + size.y - window_extra_area);
}

void dock_top_painter::adjust(ImRect* client_window)
{
	dock->set_position(client_window->Min.x, client_window->Min.y);
	dock->set_width(client_window->Max.x - dock->position.x);
	dock_horizontal_painter::adjust();
	client_window->Min.y = dock->position.y + dock->size.y -
		window_extra_area + get_border_width();
}

#pragma endregion


#pragma region dock_bottom_painter

void dock_bottom_painter::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	float parent_height = dock->position.y + dock->size.y;
	dock->position.y = mouse_position.y + mouse_clicked_position.y -
		window_extra_area;
	dock->size.y = parent_height - dock->position.y;
}

ImRect dock_bottom_painter::get_border_rectangle(dock_draw_mode mode)
{
	float line_width = get_border_width(mode);
	ImVec2 position = dock->position;
	ImVec2 size = dock->size;
	return ImRect(position.x,
		position.y + window_extra_area - line_width,
		position.x + size.x,
		position.y + window_extra_area);
}

ImRect dock_bottom_painter::get_inner_rectangle()
{
	ImVec2 position = dock->position;
	ImVec2 size = dock->size;
	return ImRect(position.x, position.y + window_extra_area,
		position.x + size.x,
		position.y + size.y);
}

void dock_bottom_painter::adjust(ImRect* client_window)
{
	dock->set_position(client_window->Min.x, client_window->Max.y
		- dock->size.y);
	dock->set_width(client_window->Max.x);
	dock_horizontal_painter::adjust();
	client_window->Max.y -= dock->size.y - window_extra_area + get_border_width();
}

#pragma endregion


#pragma region dock_fill_painter

ImRect dock_fill_painter::get_border_rectangle(dock_draw_mode)
{
	return ImRect();
}

ImRect dock_fill_painter::get_inner_rectangle()
{
	ImVec2 position = dock->position;
	ImVec2 size = dock->size;
	return ImRect(position.x, position.y,
		position.x + size.x,
		position.y + size.y);
}


void dock_fill_painter::adjust(ImRect* client_window)
{
	dock->set_position(client_window->Min.x, client_window->Min.y);
	dock->set_height(client_window->GetHeight());
	dock->set_width(client_window->GetWidth());
	adjust();
}

void dock_fill_painter::adjust()
{
	if (sleep > 0)
	{
		sleep--;
		return;
	}
	if (!dock->windows.size())
		return;

	for (auto window : dock->windows)
	{
		if (window->is_moving())
			continue;
		if (!window->is_collapsed())
			window->set_height(window->get_current_height());
		window->set_width(window->get_current_width());

		if (window->get_position().x < 1.0f)
			window->set_position(20.0f, 20.0f);

		ImVec2 position = window->get_position();
		float new_x = position.x, new_y = position.y;
		bool need_reposition = false;
		if (position.x < dock->position.x)
		{
			new_x = dock->position.x;
			need_reposition = true;
		}
		if (position.y < dock->position.y)
		{
			new_y = dock->position.y;
			need_reposition = true;
		}
		if (need_reposition)
			window->set_position(new_x, new_y);

		position = window->get_position();
		if (position.x + window->get_width() > dock->position.x + dock->size.x)
			window->set_width(dock->position.x + dock->size.x -
			position.x);
		if (position.y + window->get_height() > dock->position.y + dock->size.y)
			window->set_height(dock->position.y + dock->size.y -
			position.y);
	}
}

float dock_fill_painter::get_inner_width()
{
	return dock->get_width();
}

float dock_fill_painter::get_inner_height()
{
	return dock->get_height();
}

void dock_fill_painter::make_space(window*)
{

}

void dock_fill_painter::resize(ImVec2, ImVec2)
{

}

void dock_fill_painter::draw_window_collapsed(window* _window)
{

}

#pragma endregion

}
