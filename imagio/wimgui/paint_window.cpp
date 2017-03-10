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
	painter->draw_zero_cross();
	painter->draw_axes();
	for (float x = 0.0f, y = 0.0f, z = 0.0f;
		x <= 1000.0f; x += 5.0f, y += 5.0f, z += 5.0f)
	{
		painter->draw_point(Vector3f(x, y, z), ImColor(x / 1000.0f, y / 1000.0f, z / 1000.0f));
	}
}

}