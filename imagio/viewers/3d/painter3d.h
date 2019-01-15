#pragma once

#ifndef WIMGUI_3DPAINT_H
#define WIMGUI_3DPAINT_H

#include <vector> 
#include <string>
#include <fstream>

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

#include "view3d.h"
#include "gltool/gltool.h"


#undef min
#undef max

#define radians(angle) (angle * 3.141592653589793f / 180.0f)

namespace wimgui
{

struct vertex
{
    float position_x;
    float position_y;
    float position_z;
    float color_r;
    float color_g;
    float color_b;
    float size;
};


class view3d;


class painter3d
{
private:
	GLuint vertex_buffer;
	GLuint vertex_array;
	gltool::program program;
	vertex* vertices;
	unsigned int vertex_index = 0;

	glm::mat4 temp_model_matrix, model_matrix, transformation_matrix;

	void stop_rotating();
	void stop_moving();

#define MAX_VERTICES 64000
public:
	painter3d()
	{
		vertices = new vertex[MAX_VERTICES * sizeof(vertex)];
		init_painter();
	}
	~painter3d() { delete vertices; }
	void init_painter();
	unsigned int get_max_vertices() { return MAX_VERTICES; }
	unsigned int get_max_bytes() { return MAX_VERTICES * sizeof(vertex); }
	unsigned int get_vertex_index() { return vertex_index; }
	vertex* get_vertices() { return vertices;  }
	GLuint get_vertex_buffer() { return vertex_buffer; }
	GLuint get_vertex_array() { return vertex_array; }
	gltool::program* get_program() { return &program; }
	void draw_point(float x, float y, float z, ImColor& color, float vertex_size=5.0f);
	void draw_point(glm::vec3& position, ImColor& color, float vertex_size=5.0f)
	{
		draw_point(position.x, position.y, position.z, color, vertex_size);
	}
	void move(float x, float y);
	void move(float x, float y, float z);
	void scale(float wheel);
	void set_rotation(float x, float y, float z);
	void set_translation(float x, float y, float z);
	void rotate(float x, float y);
	void rotate(float x, float y, float z);
	void clear();
	void gl_paint(view3d& view);
};

}

#endif
