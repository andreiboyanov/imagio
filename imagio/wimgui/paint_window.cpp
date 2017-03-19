#include "paint_window.h"

namespace wimgui
{

paint_window::paint_window(const char* _title) : window(_title)
{
	painter = new wimgui::painter3d(this);
	show_title(true);
	show_border(true);
	allow_resize(true);
	allow_move(true);
	show_scrollbar(true);
	allow_collapse(true);
	show_menu(false);
}

void paint_window::draw()
{
	if (ImGui::IsMouseDragging(1, 1.0f))
	{
		ImGui::Text("Draging");
		ImVec2 delta = ImGui::GetMouseDragDelta(1, 1.0f);
		ImGui::Text("(%.2f, %.2f)", delta.x, delta.y);
		painter->move(delta.x, delta.y);
	}
	if (ImGui::IsMouseReleased(1))
	{
		painter->stop_moving();
	}
	if (ImGui::IsMouseDragging(2.0f, 1.0f))
	{
		ImGui::Text("Rotating");
		ImVec2 delta = ImGui::GetMouseDragDelta(2, 1.0f);
		ImGui::Text("(%.2f, %.2f)", delta.x, delta.y);
		painter->rotate(delta.x, delta.y);
	}
	if (ImGui::IsMouseReleased(2))
	{
		painter->stop_rotating();
	}
	painter->draw_zero_cross();
	painter->draw_axes();
	for (float x = 0.0f, y = 0.0f, z = 0.0f;
		x <= 4000.0f; x += 1.0f, y += 5.0f, z += 5.0f)
	{
        Vector3f point = Vector3f(x, y, z);
        ImColor color = ImColor(x / 1000.0f, y / 1000.0f, z / 1000.0f);
		painter->draw_point(point, color);
	}
}

void paint_window::center()
{
	ImRect canvas = get_content_rectangle();
	painter->set_view_translation(canvas.GetWidth() / 2.0f, canvas.GetHeight() / 2.0f, 0.0f);
}

void paint_window::view_top()
{
	painter->set_view_rotation(radians(90.0f), 0.0f, 0.0f);
}

void paint_window::view_bottom()
{
	painter->set_view_rotation(radians(-90.0f), 0.0f, 0.0f);
}

void paint_window::view_left()
{
	painter->set_view_rotation(0.0f, radians(90.0f), 0.0f);
}

void paint_window::view_right()
{
	painter->set_view_rotation(0.0f, radians(-90.0f), 0.0f);
}

void paint_window::view_front()
{
	painter->set_view_rotation(0.0f, 0.0f, 0.0f);
}

void paint_window::view_back()
{
	painter->set_view_rotation(0.0f, radians(180.0f), 0.0f);
}

}
