#pragma once

#ifndef VIEW3D_WINDOW_H
#define VIEW3D_WINDOW_H

#include <stddef.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "wimgui/window.h"
#include "painter3d.h"

namespace wimgui {

class painter3d;

class view3d : public window
{
private:
	std::vector<painter3d*> painters;
protected:
	ImColor cross_line_color = ImColor(0.5f, 0.5f, 0.5f);
	ImColor x_axis_color = ImColor(1.0f, 0.0f, 0.0f);
	ImColor y_axis_color = ImColor(0.0f, 1.0f, 0.0f);
	ImColor z_axis_color = ImColor(0.0f, 0.0f, 1.0f);

	glm::mat4 projection_matrix, camera_matrix, view_matrix;
	glm::mat4 temp_camera_matrix;
	void stop_rotating();
	void stop_moving();

public:
	view3d(const char* _title);
	glm::mat4 get_view_matrix()
	{
		view_matrix = projection_matrix * camera_matrix;
		return view_matrix;
	}
	void gl_paint();
	void draw();
	void center();
	void view_top();
	void view_bottom();
	void view_left();
	void view_right();
	void view_front();
	void view_back();
	void init_scene();
	void set_rotation(float x, float y, float z);
	void set_translation(float x, float y, float z);
	void move(float x, float y);
	void move(float x, float y, float z);
	void scale(float wheel);
	void rotate(float x, float y);
	void rotate(float x, float y, float z);
	void draw_zero_cross();
	void add_painter(painter3d *painter);
	void remove_painter(painter3d *painter);
};

}

#endif
