#pragma once

#ifndef WIMGUI_GIZMO_PAINTER_H
#define WIMGUI_GIZMO_PAINTER_H

#include <vector> 
#include <string>
#include <fstream>
#include <memory>

#define GLM_FORCE_RADIANS
#pragma warning(push, 0)       
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include <GL/gl3w.h>

#include "painter3d.h"
#include "viewers/3d/gltool/gl_program.h"
#include "meshes/x_cone.h"
#include "meshes/y_cone.h"
#include "meshes/z_cone.h"


namespace wimgui
{

typedef std::vector<glm::vec3> vertex_array_type;


class gizmo_painter: public painter3d
{
private:
	GLuint line_array;
	GLuint line_buffer;
	GLuint line_normal_buffer;
	GLuint line_color_buffer;
	const int line_count = 3;
	const int vertices_per_line = 2;
	const float lines[18] = 
	{
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
	};
	const float line_normals[18] = 
	{
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		1.0,  0.0, 0.0,
	};
	const float line_colors[24] = 
	{
		1.0, 0.0, 0.0, 1.0,
		1.0, 0.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 1.0,
		0.0, 0.0, 1.0, 1.0,
	};

	GLuint cones_array;
	GLuint x_cone_buffer;
	GLuint x_cone_normal_buffer;
	glm::vec4 x_color;
	GLuint y_cone_buffer;
	GLuint y_cone_normal_buffer;
	glm::vec4 y_color;
	GLuint z_cone_buffer;
	GLuint z_cone_normal_buffer;
	glm::vec4 z_color;

	void bind_mesh_data(
		gl_program::gl_program& program,
		int vertice_count,
		GLuint* vertex_buffer_ptr,
		float const* vertices_ptr,
		GLuint* normal_buffer_ptr,
		float const* normals_ptr
	);


public:
	gizmo_painter() :
		x_color(1.0f, 0.0f, 0.0f, 1.0f),
		y_color(0.0f, 1.0f, 0.0f, 1.0f),
		z_color(0.0f, 0.0f, 1.0f, 1.0f)
	{
		init_painter();
	}
	virtual void clear() override;
	virtual void gl_paint(view3d& view) override;
	virtual void draw() override {};
	void init_painter();
};

}

#endif
