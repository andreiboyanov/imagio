#pragma once

#ifndef WIMGUI_PAINTER3D_H
#define WIMGUI_PAINTER3D_H

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
#include "gltool/gl_program.h"


#undef min
#undef max

#define radians(angle) (angle * 3.141592653589793f / 180.0f)

namespace wimgui
{

class view3d;


class painter3d
{
protected:
	wimgui::gl_program::gl_program program;
	glm::mat4 temp_model_matrix, model_matrix, transformation_matrix;

	void stop_rotating();
	void stop_moving();

public:
	painter3d()
	{
	}
	void move(float x, float y);
	void move(float x, float y, float z);
	void scale(float wheel);
	void set_rotation(float x, float y, float z);
	void set_translation(float x, float y, float z);
	void rotate(float x, float y);
	void rotate(float x, float y, float z);
	virtual void clear() = 0;
	virtual void draw() = 0;
	virtual void gl_paint(view3d& view) = 0;
	wimgui::gl_program::gl_program* get_program() { return &program; }
};

}

#endif
