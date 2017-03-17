#include "3dpaint.h"

#include <iostream>

using namespace Eigen;
namespace wimgui
{

void point::draw(ImDrawList* draw_list)
{
	draw_list->AddCircle(position, 1, color);
}

void line::draw(ImDrawList* draw_list)
{
	draw_list->AddLine(from, to, color);
}

void painter3d::draw_point(Vector3f& position, ImColor& color, bool translate, bool rotate)
{
	objects3d.push_back(new point(this, position, 1.0f, color, translate, rotate));
}

void painter3d::draw_line(Vector3f& from, Vector3f& to, ImColor& color, bool translate, bool rotate)
{
	objects3d.push_back(new line(this, from, to, color, translate, rotate));
}

void painter3d::draw_text(char* text, Vector3f& position, ImColor& color, bool translate, bool rotate)
{
	ImGui::SetCursorScreenPos(window_coordinates(position, translate, rotate));
	ImGui::PushStyleColor(ImGuiCol_Text, color);
	ImGui::Text(text);
	ImGui::PopStyleColor();
}

Vector3f painter3d::view_coordinates(Vector3f& point, bool translate, bool rotate)
{
	Transform<float, 3, Affine> transform = Affine3f(AngleAxisf(0.0f, Vector3f::UnitX()));
	if (rotate)
	{
		transform = move_rotation * view_rotation * transform;
	}
	if (translate)
	{
		transform = move_translation * view_translation * transform;
	}
	Vector3f view_point = transform * (point * scale3);
	return view_point;
}

Vector3f painter3d::view_coordinates(float x, float y, float z, bool translate, bool rotate)
{
	return view_coordinates(Vector3f(x, y, z), translate, rotate);
}

ImVec2 painter3d::window_coordinates(Vector3f& point, bool translate, bool rotate)
{
	ImRect content_zone = window->get_content_rectangle();
	Transform<float, 3, Affine> transform = \
		Translation3f(0.0f, content_zone.GetHeight(), 0.0f) * \
		Affine3f(AngleAxisf(radians(180), Vector3f::UnitX()));
	Vector3f view_point = view_coordinates(point, translate, rotate);
	Vector3f window_point = transform * view_point;
	return content_zone.GetTL() + ImVec2(window_point.x(), window_point.y());
}

ImVec2 painter3d::window_coordinates(float x, float y, float z, bool translate, bool rotate)
{
	return window_coordinates(Vector3f(x, y, z), translate, rotate);
}

void painter3d::draw_zero_cross()
{
	draw_line(Vector3f(-50.0f, 0.0f, 0.0f), Vector3f(50.0f, 0.0f, 0.0f), x_axis_color);
	draw_line(Vector3f(0.0f, 0.0f, -50.0f), Vector3f(0.0f, 0.0f, 50.0f), z_axis_color);
}

void painter3d::draw_axes()
{
	draw_line(Vector3f(0, 0, 0), Vector3f(100, 0, 0), x_axis_color, false, true);
	draw_text("x", Vector3f(100, 0, 0), x_axis_color, false, true);
	draw_line(Vector3f(0, 0, 0), Vector3f(0, 100, 0), y_axis_color, false, true);
	draw_text("y", Vector3f(0, 100, 0), y_axis_color, false, true);
	draw_line(Vector3f(0, 0, 0), Vector3f(0, 0, 100), z_axis_color, false, true);
	draw_text("z", Vector3f(0, 0, 100), z_axis_color, false, true);
}

void painter3d::init_view()
{
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
	vertices = new GLfloat[24]
	{
		1.0f, 2.0f, 3.0f,
		2.0f, 2.0f, 3.0f,
		3.0f, 2.0f, 3.0f,
		4.0f, 2.0f, 3.0f,
		5.0f, 2.0f, 3.0f,
		6.0f, 2.0f, 3.0f,
		7.0f, 2.0f, 3.0f,
		8.0f, 2.0f, 3.0f,
	};
	glGenBuffers(1, &vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GLuint shader_program = create_program("shaders/vert.shader", "shaders/frag.shader");
	GLint position_attribute = glGetAttribLocation(shader_program, "position");
	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_attribute);

	move_translation = Translation3f(0.0f, 0.0f, 0.0f);
	move_rotation = Affine3f(AngleAxisf(0.0f, Vector3f::UnitX()));
	view_translation = Translation3f(300.0f, 300.0f, 300.0f);
	view_rotation = Affine3f(AngleAxisf(radians(-20), Vector3f::UnitX()) *
		AngleAxisf(radians(20), Vector3f::UnitY()));
}


void painter3d::move(float x, float y)
{
	Vector3f delta = Vector3f(x, -y, 0.0f);
	move_translation = Translation3f(delta);
	recalculate();
}

void painter3d::move(float x, float y, float z)
{
	x, y, z;
}

void painter3d::stop_moving()
{
	view_translation = move_translation * view_translation;
	move_translation = Translation3f(0.0f, 0.0f, 0.0f);
	recalculate();
}

void painter3d::rotate(float x, float y)
{
	Vector3f delta = Vector3f(x, -y, 0.0f);
	move_rotation = Affine3f(AngleAxisf(y / 100.0f, Vector3f::UnitX()) *
		AngleAxisf(-x / 100.0f, Vector3f::UnitY()));
	recalculate();
}

void painter3d::rotate(float x, float y, float z)
{
	x, y, z;
}

void painter3d::stop_rotating()
{
	view_rotation = move_rotation * view_rotation;
	move_rotation = Affine3f(AngleAxisf(0.0f, Vector3f::UnitX()));
	recalculate();
}

void painter3d::scale(float wheel)
{
	scale3 += wheel / 10.0f;
	recalculate();
}

void painter3d::set_view_rotation(float x, float y, float z)
{
	view_rotation = Affine3f(AngleAxisf(x, Vector3f::UnitX()) *
		AngleAxisf(y, Vector3f::UnitY()) *
		AngleAxisf(z, Vector3f::UnitZ()));
}

void painter3d::set_view_translation(float x, float y, float z)
{
	view_translation = Translation3f(x, y, z);
}

void painter3d::clear()
{
	objects3d.clear();
}

void painter3d::draw()
{
	glBindTexture(GL_TEXTURE_2D, (GLuint)texture_id);
	// glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vertex_array_object);
	glDrawArrays(GL_POINTS, 0, 8);
	// glfwSwapBuffers();
	//for (object3d* graphic_object : objects3d)
	//{
	//	graphic_object->draw(window->get_imgui_window()->DrawList);
	//}
}

void painter3d::init_scene()
{
	clear();
	draw_zero_cross();
	draw_axes();
}

void painter3d::recalculate()
{
	for (object3d* graphic_object : objects3d)
	{
		graphic_object->recalculate();
	}
}


GLuint painter3d::load_and_compile_shader(std::string filename, GLenum shader_type) {
	std::ifstream inputfile(filename.c_str());
	std::string shader_string((std::istreambuf_iterator<char>(inputfile)),
							   std::istreambuf_iterator<char>());
	const char *src = shader_string.c_str();
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	GLint test;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
	if (!test) {
		std::cerr << "Shader compilation failed with this message:" << std::endl;
		std::vector<char> compilation_log(512);
		glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
		std::cerr << &compilation_log[0] << std::endl;
	}
	return shader;
}

GLuint painter3d::create_program(std::string vertex_shader_filename, std::string fragment_shader_filename)
{
	GLuint vertex_shader = load_and_compile_shader(vertex_shader_filename, GL_VERTEX_SHADER);
	GLuint fragment_shader = load_and_compile_shader(fragment_shader_filename, GL_FRAGMENT_SHADER);
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glLinkProgram(shader_program);
	glUseProgram(shader_program);
	return shader_program;
}

}