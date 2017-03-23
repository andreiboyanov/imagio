#pragma once

#ifndef WIMGUI_3DPAINT_H
#define WIMGUI_3DPAINT_H

#include <vector> 
#include <string>
#include <fstream>

#pragma warning(push, 0)       
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
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

using namespace Eigen;
namespace wimgui
{

class painter3d;

struct object3d
{
protected:
	painter3d* painter;
	bool translate, rotate;
public:
	object3d(painter3d* _painter, bool _translate, bool _rotate)
		: painter(_painter), translate(_translate), rotate(_rotate)
	{}
	virtual void draw() = 0;
};

class painter3d
{
private:
	wimgui::window* window;
	Affine3f view_rotation;
	Affine3f move_rotation;
	Translation3f view_translation;
	Translation3f move_translation;
	float scale3 = 1.0f;
	float scale2 = 1.0f;
	ImColor cross_line_color = ImColor(0.5f, 0.5f, 0.5f);
	ImColor x_axis_color = ImColor(1.0f, 0.0f, 0.0f);
	ImColor y_axis_color = ImColor(0.0f, 1.0f, 0.0f);
	ImColor z_axis_color = ImColor(0.0f, 0.0f, 1.0f);
	std::vector<object3d*> objects3d = std::vector<object3d*>();
	GLuint texture_id;
	GLuint vertex_buffer;
	GLuint vertex_array;
	gltool::program program;
	GLfloat *vertices;
	unsigned int vertex_index = 0;

#define MAX_VERTICES 64000
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	painter3d(wimgui::window* _window, ImTextureID _texture_id)
	{
		window = _window;
		texture_id = (GLuint)_texture_id;
		vertices = new GLfloat[3 * MAX_VERTICES];
		init_view();
	}
	~painter3d() { delete vertices; }
	void init_view();
	unsigned int get_max_vertices() { return MAX_VERTICES; }
	unsigned int get_vertex_index() { return vertex_index; }
	GLfloat* get_vertices() { return vertices;  }
	GLuint get_texture_id() { return texture_id; }
	GLuint get_vertex_buffer() { return vertex_buffer; }
	GLuint get_vertex_array() { return vertex_array; }
	gltool::program* get_program() { return &program; }
	void draw_line(Vector3f& from, Vector3f& to, ImColor& color);
	void draw_point(Vector3f& poition, ImColor& color);
	void draw_point(float x, float y, float z, ImColor& color);
	void draw_text(char* text, Vector3f& position, ImColor& color);
	ImVec2 window_coordinates(float x, float y, float z);
	ImVec2 window_coordinates(Vector3f& point);
	Vector3f view_coordinates(float x, float y, float z);
	Vector3f view_coordinates(Vector3f& point);
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

struct point : public object3d
{
public:
	float size;
	Vector3f position3d;
	ImVec2 position;
	ImColor color;
	point(painter3d* _painter, Vector3f& _position3d, float _size, ImColor _color,
		  bool _translate = true, bool _rotate = true)
		: object3d(_painter, _translate, _rotate), position3d(_position3d), size(_size), color(_color)
	{}
	virtual void draw();
};

struct line : public object3d
{
public:
	Vector3f from3d, to3d;
	ImVec2 from, to;
	ImColor color;
	line(painter3d* _painter, Vector3f _from3d, Vector3f _to3d, ImColor _color,
		 bool _translate = true, bool _rotate = true)
		: object3d(_painter, _translate, _rotate), from3d(_from3d), to3d(_to3d), color(_color)
	{}
	virtual void draw();
};


}

#endif
