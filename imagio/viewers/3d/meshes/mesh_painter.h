#pragma once

#ifndef WIMGUI_MESH_PAINTER_H
#define WIMGUI_MESH_PAINTER_H

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

#include "../painter3d.h"
#include "gl_mesh.h"
#include  "cone.h"


namespace wimgui
{

typedef std::vector<glm::vec3> vertex_array_type;


class mesh_painter: public painter3d
{
private:
	GLuint vertex_buffer;
	GLuint vertex_array;
	GLuint normal_buffer;
	mesh_program program;

public:
	mesh_painter() 
	{
		init_painter();
	}
	GLuint get_vertex_buffer() { return vertex_buffer; }
	GLuint get_vertex_array() { return vertex_array; }
	mesh_program* get_program() { return &program; }
	virtual void clear() override;
	virtual void gl_paint(view3d& view) override;
	virtual void draw() override {};
	void init_painter();
};

}

#endif
