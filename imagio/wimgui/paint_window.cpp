#include "paint_window.h"

namespace wimgui
{

paint_window::paint_window(const char* _title) : window(_title)
{
	painter = new wimgui::painter3d(this, (ImTextureID)0x99);
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
	ImTextureID texture_id = (ImTextureID)0x99;
	ImGui::Image((ImTextureID)painter->get_texture_id(), get_content_rectangle().GetSize());
	//if (ImGui::IsMouseDragging(1, 1.0f))
	//{
	//	ImGui::Text("Draging");
	//	ImVec2 delta = ImGui::GetMouseDragDelta(1, 1.0f);
	//	ImGui::Text("(%.2f, %.2f)", delta.x, delta.y);
	//	painter->move(delta.x, delta.y);
	//}
	//if (ImGui::IsMouseReleased(1))
	//{
	//	painter->stop_moving();
	//}
	//if (ImGui::IsMouseDragging(2.0f, 1.0f))
	//{
	//	ImGui::Text("Rotating");
	//	ImVec2 delta = ImGui::GetMouseDragDelta(2, 1.0f);
	//	ImGui::Text("(%.2f, %.2f)", delta.x, delta.y);
	//	painter->rotate(delta.x, delta.y);
	//}
	//if (ImGui::IsMouseReleased(2))
	//{
	//	painter->stop_rotating();
	//}

	painter->draw();

	//float mouse_wheel = ImGui::GetIO().MouseWheel;
	//ImGui::Text("%f", mouse_wheel);
	//painter->scale(mouse_wheel);
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

void paint_window::init_scene()
{
	painter->init_scene();
}

}