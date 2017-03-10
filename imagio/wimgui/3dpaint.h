#pragma once

#ifndef WIMGUI_3DPAINT_H
#define WIMGUI_3DPAINT_H

#include <Eigen/Dense>
#include <Eigen/Geometry>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include "window.h"

#define radians(angle) (angle * 3.141592653589793f / 180.0f)


using namespace Eigen;
namespace wimgui
{

class painter3d
{
private:
	window* window;
	Affine3f view_rotation;
	Translation3f view_translation;
	Translation3f move_translation;
	float scale3 = 1.0f;
	float scale2 = 1.0f;
	ImColor cross_line_color = ImColor(0.5f, 0.5f, 0.5f);
	ImColor x_axis_color = ImColor(1.0f, 0.0f, 0.0f);
	ImColor y_axis_color = ImColor(0.0f, 1.0f, 0.0f);
	ImColor z_axis_color = ImColor(0.0f, 0.0f, 1.0f);

	void init_view();
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	// FIXME: Clean a bit the concept of rotation_only transformation
	// (needed for the axes...)
	painter3d(wimgui::window* _window) { window = _window; init_view();  }
	void draw_line(Vector3f& from, Vector3f& to, ImColor& color, bool do_transform=true);
	void draw_point(Vector3f& poition, ImColor& color, bool do_transform=true);
	void draw_text(char* text, Vector3f& position, ImColor& color, bool do_transform=true);
	ImVec2 window_coordinates(float x, float y, float z, bool do_transform=true);
	ImVec2 window_coordinates(Vector3f& point, bool do_transform=true);
	Vector3f view_coordinates(float x, float y, float z, bool rotation_only=false);
	Vector3f view_coordinates(Vector3f& point, bool rotation_only=false);
	void draw_zero_cross();
	void draw_axes();
	void move(float x, float y);
	void move(float x, float y, float z);
	void stop_moving();
};

}

#endif