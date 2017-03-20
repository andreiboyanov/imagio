#pragma once

#ifndef WIMGUI_3DPAINT_H
#define WIMGUI_3DPAINT_H

#include <vector> 
#include <string>
#include <fstream>

#include <Eigen/Dense>
#include <Eigen/Geometry>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include <GL/gl3w.h>

#include "window.h"

#undef min
#undef max

#define radians(angle) (angle * 3.141592653589793f / 180.0f)

using namespace Eigen;
namespace wimgui
{

struct painter3d;

struct object3d
{
protected:
	painter3d* painter;
	bool translate, rotate;
public:
	object3d(painter3d* _painter, bool _translate, bool _rotate)
		: painter(_painter), translate(_translate), rotate(_rotate) {}
	virtual void draw() = 0;
	virtual void recalculate() = 0;
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
	GLuint vertex_array_object;
	GLuint vertex_buffer_object;
	GLfloat *vertices;

public:
	void init_view();
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	painter3d(wimgui::window* _window, ImTextureID _texture_id) {
		window = _window;
		texture_id = (GLuint)_texture_id;
		init_view();
	}
	GLuint get_texture_id() { return texture_id;  }
	void draw_line(Vector3f& from, Vector3f& to, ImColor& color, bool translate=true, bool rotate=true, bool scale=true);
	void draw_point(Vector3f& poition, ImColor& color, bool translate=true, bool rotate=true, bool scale=true);
	void draw_text(char* text, Vector3f& position, ImColor& color, bool translate=true, bool rotate=true, bool scale=true);
	ImVec2 window_coordinates(float x, float y, float z, bool translate=true, bool rotate=true, bool scale=true);
	ImVec2 window_coordinates(Vector3f& point, bool translate=true, bool rotate=true, bool scale=true);
	Vector3f view_coordinates(float x, float y, float z, bool translate=true, bool rotate=true, bool scale=true);
	Vector3f view_coordinates(Vector3f& point, bool translate=true, bool rotate=true, bool scale=true);
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
	void recalculate();
protected:
	GLuint load_and_compile_shader(std::string filename, GLenum shader_type);
	GLuint create_program(std::string vertex_shader_filename, std::string fragment_shader_filename);
};

struct point : public object3d
{
public:
	float size;
	Vector3f position3d;
	ImVec2 position;
	ImColor color;
	point(painter3d* _painter, Vector3f& _position3d, float _size, ImColor _color,
		bool _translate=true, bool _rotate=true)
		: object3d(_painter, _translate, _rotate), position3d(_position3d), size(_size), color(_color)
	{
		recalculate();
	}
	virtual void draw();
	virtual void recalculate() { position = painter->window_coordinates(position3d, translate, rotate); }
};

struct line : public object3d
{
public:
	Vector3f from3d, to3d;
	ImVec2 from, to;
	ImColor color;
	line(painter3d* _painter, Vector3f _from3d, Vector3f _to3d, ImColor _color,
		bool _translate=true, bool _rotate=true)
		: object3d(_painter, _translate, _rotate), from3d(_from3d), to3d(_to3d), color(_color)
	{
		recalculate();
	}
	virtual void draw();
	virtual void recalculate() {
		from = painter->window_coordinates(from3d, translate, rotate);
		to = painter->window_coordinates(to3d, translate, rotate);
	}
};


}

#endif
