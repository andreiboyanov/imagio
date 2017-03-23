#pragma once

#ifndef WIMGUI_3DPAINT_H
#define WIMGUI_3DPAINT_H

#include <vector> 
#include <string>
#include <fstream>

#pragma warning(push, 0)       
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include <GL/gl3w.h>

#include "../wimgui/window.h"
#include "gltool/gltool.h"


#undef min
#undef max

#define radians(angle) (angle * 3.141592653589793f / 180.0f)

namespace wimgui
{

class painter3d
{
private:
	wimgui::window* window;
	ImColor cross_line_color = ImColor(0.5f, 0.5f, 0.5f);
	ImColor x_axis_color = ImColor(1.0f, 0.0f, 0.0f);
	ImColor y_axis_color = ImColor(0.0f, 1.0f, 0.0f);
	ImColor z_axis_color = ImColor(0.0f, 0.0f, 1.0f);

	GLuint vertex_buffer;
	GLuint vertex_array;
	gltool::program program;
	GLfloat *vertices;
	unsigned int vertex_index = 0;

	glm::mat4 projection_matrix, view_matrix, model_matrix, transformation_matrix;

#define MAX_VERTICES 64000
public:
	painter3d(wimgui::window* _window)
	{
		window = _window;
		vertices = new GLfloat[3 * MAX_VERTICES];
		init_view();
	}
	~painter3d() { delete vertices; }
	void init_view();
	float* get_transformation_pointer()
	{ 
		transformation_matrix =  projection_matrix * view_matrix * model_matrix; 
		return glm::value_ptr(transformation_matrix);
	}
	unsigned int get_max_vertices() { return MAX_VERTICES; }
	unsigned int get_vertex_index() { return vertex_index; }
	GLfloat* get_vertices() { return vertices;  }
	GLuint get_vertex_buffer() { return vertex_buffer; }
	GLuint get_vertex_array() { return vertex_array; }
	gltool::program* get_program() { return &program; }
	void draw_point(float x, float y, float z, ImColor& color);
	void draw_zero_cross();
	void draw_axes();
	void move(float x, float y);
	void move(float x, float y, float z);
	void scale(float wheel);
	void stop_moving();
	void rotate(float x, float y);
	void rotate(float x, float y, float z);
	void stop_rotating();
	void set_view_rotation(float x, float y, float z);
	void set_view_translation(float x, float y, float z);
	void clear();
	void draw();
	void init_scene();
	wimgui::window* get_window() { return window; }
};

}

#endif
