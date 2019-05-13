#pragma once

#ifndef WIMGUI_POINTCLOUD_PAINTER_H
#define WIMGUI_POINTCLOUD_PAINTER_H

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
#include "gltool/gl_pointcloud.h"
#include "painter3d.h"


namespace wimgui
{

typedef std::vector<glm::vec3> vertex_array_type;


class pointcloud_painter: public painter3d
{
private:
	GLuint vertex_buffer;
	GLuint vertex_array;
	gltool::program program;
	std::weak_ptr<vertex_array_type> vertex_pointer;

#define MAX_VERTICES 64000
public:
	pointcloud_painter()
	{
		init_painter();
	}
	void init_painter();
	unsigned int get_max_vertices() { return MAX_VERTICES; }
	GLuint get_vertex_buffer() { return vertex_buffer; }
	GLuint get_vertex_array() { return vertex_array; }
	gltool::program* get_program() { return &program; }
	virtual void clear() override;
	virtual void gl_paint(view3d& view) override;
	virtual void draw() override {};
	void set_data(std::shared_ptr<vertex_array_type> point_cloud_pointer);
};

}

#endif
