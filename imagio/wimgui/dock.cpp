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
		break;
	case dock_right:
		break;
	}
}

void docker::add_window(window *_window)
{
	windows.push_back(_window);
	_window->dock_to(this);
	painter->adjust();
}

void docker::remove_window(window *_window)
{
	_window->dock_to(nullptr);
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
		_window->draw_imgui();
	}
}

void docker::draw()
{
	ImGuiContext& context = *GImGui;
	const ImRect border = painter->get_border_rectangle();
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

float dock_painter::border_width()
{
	return (dock->border_hovered || dock->border_held) ?  hover_delta : 2.0f;
}

#pragma endregion


#pragma region dock_left_painter

void dock_left_painter::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	dock->set_width(mouse_position.x +
		-mouse_clicked_position.x +
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
		float width = get_inner_width();
		ImVec2 position = dock->get_position();
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGrip),
			ImVec2(width + (line_width / 2), position.y),
			ImVec2(width + (line_width / 2),
						get_inner_height() + position.y),
						line_width);
		break;
	}
	case draw_hovered:
	{
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripHovered), get_border_rectangle());
		break;
	}
	case draw_resizing:
	{
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripActive), get_border_rectangle());
		break;
	}
	case draw_none:
	{
		break;
	}

	}
}

void dock_left_painter::adjust(ImRect* client_window)
{
	dock->set_position(client_window->Min.x, client_window->Min.y);
	dock->set_height(client_window->GetHeight());
	adjust();
	client_window->Min.x = dock->get_width() +
		dock->get_position().x -
		window_extra_area + border_width();
}

void dock_left_painter::adjust()
{
	if (sleep > 0) {
		sleep--;
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

#pragma endregion


#pragma region dock_bottom_painter

void dock_bottom_painter::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	float parent_height = dock->position.y + dock->size.y;
	dock->position.y = mouse_position.y + mouse_clicked_position.y -
		window_extra_area;
	dock->size.y = parent_height - dock->position.y;
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
		ImVec2 position = dock->get_position();
		float width = get_inner_width();
		float y = position.y + window_extra_area - line_width / 2;
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGrip),
			ImVec2(position.x, y),
			ImVec2(position.x + width, y),
			line_width);
		break;
	}
	case draw_hovered:
	{
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripHovered), get_border_rectangle());
		break;
	}
	case draw_resizing:
	{
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripActive), get_border_rectangle());
		break;
	}
	case draw_none:
	{
		break;
	}

	}
}

void dock_bottom_painter::adjust(ImRect* client_window)
{
	dock->set_position(client_window->Min.x, client_window->Max.y
		- dock->size.y);
	dock->set_width(client_window->Max.x);
	adjust();
	client_window->Max.y -= dock->size.y - window_extra_area + border_width();
}

void dock_bottom_painter::adjust()
{
	if (sleep > 0) {
		sleep--;
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

#pragma endregion

#pragma region dock_top_painter

void dock_top_painter::resize(ImVec2 mouse_position, ImVec2 mouse_clicked_position)
{
	dock->size.y = mouse_position.y + mouse_clicked_position.y +
		window_extra_area - dock->position.y;
}

ImRect dock_top_painter::get_border_rectangle()
{
	ImVec2 position = dock->get_position();
	ImVec2 size = dock->get_size();
	return ImRect(position.x,
					position.y + size.y - window_extra_area,
					position.x + size.x,
					position.y + size.y - window_extra_area + hover_delta);
}

// FIXME: This method should be in the dock_painter class.
void dock_top_painter::draw_border(dock_draw_mode mode)
{
	switch (mode)
	{
	case draw_normal:
	{
		float line_width = 2.0f;
		ImVec2 position = dock->get_position();
		ImVec2 size = dock->get_size();
		float width = get_inner_width();
		float y = position.y + size.y - window_extra_area + line_width / 2;
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGrip),
							ImVec2(position.x, y),
							ImVec2(position.x + width, y),
							line_width);
		break;
	}
	case draw_hovered:
	{
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripHovered), get_border_rectangle());
		break;
	}
	case draw_resizing:
	{
		draw_border2(ImGui::GetColorU32(ImGuiCol_ResizeGripActive), get_border_rectangle());
		break;
	}
	case draw_none:
	{
		break;
	}

	}
}

void dock_top_painter::adjust(ImRect* client_window)
{
	dock->set_position(client_window->Min.x, client_window->Min.y);
	dock->set_width(client_window->Max.x - dock->position.x);
	adjust();
}

// FIXME: Need intermediate dock_horizontal_painter
// These methodis are the same as in dock_botoom_painter
void dock_top_painter::adjust()
{
	if (sleep > 0) {
		sleep--;
		return;
	}
	if (!dock->windows.size())
		return;
	float last_x = dock->position.x;
	for (auto window : dock->windows)
	{
		window->set_height(dock->get_inner_height());
		window->set_position(last_x, dock->position.y);

		float window_width = window->get_current_width();
		if (!window->is_collapsed())
			window->set_width(window_width);

		last_x += window_width;
	}
}

float dock_top_painter::get_inner_width()
{
	return dock->get_width();
}

float dock_top_painter::get_inner_height()
{
	return dock->get_height() - window_extra_area;
}

#pragma endregion
}
