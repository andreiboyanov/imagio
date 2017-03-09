#include "3dpaint.h"

#include <iostream>

using namespace Eigen;
namespace wimgui
{

void painter3d::draw_point(Vector3f& position, ImColor& color, bool do_transform)
{
	ImDrawList* draw_list = ImGui::GetCurrentWindow()->DrawList;
	ImVec2 point = window_coordinates(position, do_transform);
	draw_list->AddCircle(point, 1.0f, color, 12, 1.0f);
}

void painter3d::draw_line(Vector3f& from, Vector3f& to, ImColor& color, bool do_transform)
{
	ImDrawList* draw_list = ImGui::GetCurrentWindow()->DrawList;
	ImVec2 from_point = window_coordinates(from, do_transform);
	ImVec2 to_point = window_coordinates(to, do_transform);
	draw_list->AddLine(from_point, to_point, color);
}

void painter3d::draw_text(char* text, Vector3f& position, ImColor& color, bool do_transform)
{
	ImGui::SetCursorScreenPos(window_coordinates(position, do_transform));
	ImGui::Text(text);
}

Vector3f painter3d::view_coordinates(Vector3f& point, bool rotation_only)
{
	Transform<float, 3, Affine> transform = view_rotation;
	if (!rotation_only)
		transform = view_translation * transform;
	Vector3f view_point = transform * point;
	return view_point;
}

Vector3f painter3d::view_coordinates(float x, float y, float z, bool rotation_only)
{
	return view_coordinates(Vector3f(x, y, z), rotation_only);
}

ImVec2 painter3d::window_coordinates(Vector3f& point, bool do_transform)
{
	ImRect content_zone = window->get_content_rectangle();
	Transform<float, 3, Affine> transform = \
		Translation3f(0.0f, content_zone.GetHeight(), 0.0f) * \
		Affine3f(AngleAxisf(radians(180), Vector3f::UnitX()));
	Vector3f window_point = transform * view_coordinates(point, !do_transform);
	return content_zone.GetTL() + ImVec2(window_point.x(), window_point.y());
}

ImVec2 painter3d::window_coordinates(float x, float y, float z, bool do_transform)
{
	return window_coordinates(Vector3f(x, y, z), do_transform);
}

void painter3d::draw_zero_cross()
{
	draw_line(Vector3f(-50.0f, 0.0f, 0.0f), Vector3f(50.0f, 0.0f, 0.0f), cross_line_color);
	draw_line(Vector3f(0.0f, 0.0f, -50.0f), Vector3f(0.0f, 0.0f, 50.0f), cross_line_color);
}

void painter3d::draw_axes()
{
	draw_line(Vector3f(0, 0, 0), Vector3f(100, 0, 0), x_axis_color, false);
	draw_text("x", Vector3f(100, 0, 0), x_axis_color, false);
	draw_line(Vector3f(0, 0, 0), Vector3f(0, 100, 0), y_axis_color, false);
	draw_text("y", Vector3f(0, 100, 0), y_axis_color, false);
	draw_line(Vector3f(0, 0, 0), Vector3f(0, 0, 100), z_axis_color, false);
	draw_text("z", Vector3f(0, 0, 100), z_axis_color, false);
}

void painter3d::init_view()
{
	view_translation = Translation3f(300.0f, 300.0f, 300.0f);
	view_rotation = Affine3f(AngleAxisf(radians(-90), Vector3f::UnitX())) * \
					Affine3f(AngleAxisf(radians(0), Vector3f::UnitY())); 
}

}