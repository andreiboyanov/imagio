#include "3dpaint.h"

#include <iostream>

using namespace Eigen;
namespace wimgui
{

void painter3d::draw_point(Vector3f& position, ImColor& color, bool do_transform)
{
	ImDrawList* draw_list = ImGui::GetCurrentWindow()->DrawList;
	ImVec2 from_point = window_coordinates(position, do_transform);
	ImVec2 to_point = from_point + ImVec2(1.0f, 1.0f);
	draw_list->AddLine(from_point, to_point, color);
}

void painter3d::draw_line(Vector3f& from, Vector3f& to, ImColor& color, bool do_transform)
{
	ImDrawList* draw_list = ImGui::GetCurrentWindow()->DrawList;
	ImVec2 from_point = window_coordinates(from, do_transform);
	ImVec2 to_point = window_coordinates(to, do_transform);
	draw_list->AddLine(from_point, to_point, color);
}

void painter3d::draw_text(const char* text, Vector3f& position, ImColor& color, bool do_transform)
{
	ImGui::SetCursorScreenPos(window_coordinates(position, do_transform));
	ImGui::PushStyleColor(ImGuiCol_Text, color);
	ImGui::Text("%s", text);
	ImGui::PopStyleColor();
}

Vector3f painter3d::view_coordinates(Vector3f& point, bool rotation_only)
{
	Transform<float, 3, Affine> transform = move_rotation * view_rotation;
	if (!rotation_only)
	{
		transform = move_translation * view_translation * transform;
	}
	Vector3f view_point = transform * point;
	return view_point;
}

Vector3f painter3d::view_coordinates(float x, float y, float z, bool rotation_only)
{
    Vector3f vector = Vector3f(x, y, z);
	return view_coordinates(vector, rotation_only);
}

ImVec2 painter3d::window_coordinates(Vector3f& point, bool do_transform)
{
	ImRect content_zone = window->get_content_rectangle();
	Transform<float, 3, Affine> transform = \
		Translation3f(0.0f, content_zone.GetHeight(), 0.0f) * \
		Affine3f(AngleAxisf(radians(180), Vector3f::UnitX()));
	Vector3f view_point = view_coordinates(point, !do_transform);
	Vector3f window_point = transform * view_point;
	return content_zone.GetTL() + ImVec2(window_point.x(), window_point.y());
}

ImVec2 painter3d::window_coordinates(float x, float y, float z, bool do_transform)
{
    Vector3f vector = Vector3f(x, y, z);
	return window_coordinates(vector, do_transform);
}

void painter3d::draw_zero_cross()
{
    Vector3f from1 = Vector3f(-50.0f, 0.0f, 0.0f);
    Vector3f to1 = Vector3f(50.0f, 0.0f, 0.0f);
    Vector3f from2 = Vector3f(0.0f, 0.0f, -50.0f);
    Vector3f to2 = Vector3f(0.0f, 0.0f, 50.0f);
	draw_line(from1, to1, x_axis_color);
	draw_line(from2, to2, z_axis_color);
}

void painter3d::draw_axes()
{
    Vector3f from_point = Vector3f(0, 0, 0);
    Vector3f to_point = Vector3f(100, 0, 0);
	draw_line(from_point, to_point, x_axis_color, false);
	draw_text("x", to_point, x_axis_color, false);

    from_point = Vector3f(0, 0, 0);
    to_point = Vector3f(0, 100, 0);
	draw_line(from_point, to_point, y_axis_color, false);
	draw_text("y", to_point, y_axis_color, false);

    from_point = Vector3f(0, 0, 0);
    to_point = Vector3f(0, 0, 100);
	draw_line(from_point, to_point, z_axis_color, false);
	draw_text("z", to_point, z_axis_color, false);
}

void painter3d::init_view()
{
	move_translation = Translation3f(0.0f, 0.0f, 0.0f);
	move_rotation = Affine3f(AngleAxisf(0.0f, Vector3f::UnitX()));
	view_translation = Translation3f(300.0f, 300.0f, 300.0f);
	view_rotation = Affine3f(AngleAxisf(radians(-20), Vector3f::UnitX()) * 
							 AngleAxisf(radians(20), Vector3f::UnitY())); 
}


void painter3d::move(float x, float y)
{
	Vector3f delta = Vector3f(x, -y, 0.0f);
	move_translation = Translation3f(delta);
}

// void painter3d::move(float x, float y, float z)
// {
// 	x, y, z;
// }

void painter3d::stop_moving()
{
	view_translation = move_translation * view_translation;
	move_translation = Translation3f(0.0f, 0.0f, 0.0f);
}

void painter3d::rotate(float x, float y)
{
	move_rotation = Affine3f(AngleAxisf(-y / 100.0f, Vector3f::UnitX()) * 
							 AngleAxisf(-x / 100.0f, Vector3f::UnitY()));
}

// void painter3d::rotate(float x, float y, float z)
// {
// 	x, y, z;
// }

void painter3d::stop_rotating()
{
	view_rotation = move_rotation * view_rotation;
	move_rotation = Affine3f(AngleAxisf(0.0f, Vector3f::UnitX()));
}

void painter3d::set_view_rotation(float x, float y, float z)
{
	view_rotation = Affine3f(AngleAxisf(x, Vector3f::UnitX()) * 
							 AngleAxisf(y, Vector3f::UnitY()) *
							 AngleAxisf(z, Vector3f::UnitZ()));
}

void painter3d::set_view_translation(float x, float y, float z)
{
	view_translation = Translation3f(x, y, z);
}

}
