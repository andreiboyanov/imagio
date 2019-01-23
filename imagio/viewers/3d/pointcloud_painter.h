#pragma once

#ifndef WIMGUI_POINTCLOUD_PAINTER_H
#define WIMGUI_POINTCLOUD_PAINTER_H

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

#include "painter3d.h"
#include "gltool/gltool.h"
#include "painter3d.h"


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


class pointcloud_painter: public painter3d
{
private:
	GLuint vertex_buffer;
	GLuint vertex_array;
	gltool::program program;
	vertex* vertices;
	unsigned int vertex_index = 0;

#define MAX_VERTICES 64000
public:
	pointcloud_painter()
	{
		vertices = new vertex[MAX_VERTICES * sizeof(vertex)];
		init_painter();
	}
	~pointcloud_painter() { delete vertices; }
	void init_painter();
	unsigned int get_max_vertices() { return MAX_VERTICES; }
	unsigned int get_max_bytes() { return MAX_VERTICES * sizeof(vertex); }
	unsigned int get_vertex_index() { return vertex_index; }
	vertex* get_vertices() { return vertices;  }
	GLuint get_vertex_buffer() { return vertex_buffer; }
	GLuint get_vertex_array() { return vertex_array; }
	gltool::program* get_program() { return &program; }
	void draw_point(float x, float y, float z, const ImColor& color, float vertex_size=5.0f);
	void draw_point(glm::vec3& position, const ImColor& color, float vertex_size=5.0f)
	{
		draw_point(position.x, position.y, position.z, color, vertex_size);
	}
	virtual void clear() override;
	virtual void gl_paint(view3d& view) override;
	virtual void draw() override {};
};

}

#endif
