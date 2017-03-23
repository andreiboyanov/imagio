#include "3dpaint.h"

#include <iostream>

using namespace Eigen;
namespace wimgui
{

void point::draw()
{

}

void line::draw()
{

}

void painter3d::draw_point(Vector3f& position, ImColor& color)
{
	draw_point(position.x(), position.y(), position.z(), color);
}

void painter3d::draw_point(float x, float y, float z, ImColor& color)
{
	if(color.Value.x > 0) {}
	if(vertex_array + 3 > get_max_vertices()) { return; }
	ImRect canvas = window->get_content_rectangle();
	vertices[vertex_index++] = -0.5f + 5.0f * x / (2.0f * canvas.GetWidth());
	vertices[vertex_index++] = 0.5f + 5.0f * y / (2.0f * canvas.GetHeight());
	vertices[vertex_index++] = z - z;
}


void painter3d::draw_line(Vector3f& from, Vector3f& to, ImColor& color)
{
	if(from.x() > 0) {}
	if(to.x() > 0) {}
	if(color.Value.x > 0) {}
}

void painter3d::draw_text(char* text, Vector3f& position, ImColor& color)
{
	if(text) {}
	if(position.x() > 0) {}
	if(color.Value.x > 0) {}
}

Vector3f painter3d::view_coordinates(Vector3f& point)
{
	Transform<float, 3, Affine> transform = Affine3f(AngleAxisf(0.0f, Vector3f::UnitX()));
	transform = move_rotation * view_rotation * transform;
	transform = move_translation * view_translation * transform;
	Vector3f view_point = point;
	view_point *= scale3;
	view_point = transform * view_point;
	return view_point;
}

Vector3f painter3d::view_coordinates(float x, float y, float z)
{
	Vector3f vector = Vector3f(x, y, z);
	return view_coordinates(vector);
}

ImVec2 painter3d::window_coordinates(Vector3f& point)
{
	ImRect content_zone = window->get_content_rectangle();
	Transform<float, 3, Affine> transform = \
		Translation3f(0.0f, content_zone.GetHeight(), 0.0f) * \
		Affine3f(AngleAxisf(radians(180), Vector3f::UnitX()));
	Vector3f view_point = view_coordinates(point);
	Vector3f window_point = transform * view_point;
	return content_zone.GetTL() + ImVec2(window_point.x(), window_point.y());
}

ImVec2 painter3d::window_coordinates(float x, float y, float z)
{
	Vector3f vector = Vector3f(x, y, z);
	return window_coordinates(vector);
}

void painter3d::draw_zero_cross()
{
	Vector3f from1 = Vector3f(-50.0f, 0.0f, 0.0f);
	Vector3f to1 = Vector3f(50.0f, 0.0f, 0.0f);
	Vector3f from2 = Vector3f(0.0f, 0.0f, -50.0f);
	Vector3f to2 = Vector3f(0.0f, 0.0f, 50.0f);
	draw_line(from1, to1, x_axis_color);
	draw_line(from2, to2, z_axis_color);
}

void painter3d::draw_axes()
{
	Vector3f from_point = Vector3f(0, 0, 0);
	Vector3f to_point = Vector3f(100, 0, 0);
	draw_line(from_point, to_point, x_axis_color);
	draw_text("x", to_point, x_axis_color);

	from_point = Vector3f(0, 0, 0);
	to_point = Vector3f(0, 100, 0);
	draw_line(from_point, to_point, y_axis_color);
	draw_text("y", to_point, y_axis_color);

	from_point = Vector3f(0, 0, 0);
	to_point = Vector3f(0, 0, 100);
	draw_line(from_point, to_point, z_axis_color);
	draw_text("z", to_point, z_axis_color);
}

void painter3d::move(float x, float y)
{
	Vector3f delta = Vector3f(x, -y, 0.0f);
	move_translation = Translation3f(delta);
	// recalculate();
}

void painter3d::move(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

void painter3d::stop_moving()
{
	view_translation = move_translation * view_translation;
	move_translation = Translation3f(0.0f, 0.0f, 0.0f);
}

void painter3d::rotate(float x, float y)
{
	move_rotation = Affine3f(AngleAxisf(-y / 100.0f, Vector3f::UnitX()) *
							 AngleAxisf(-x / 100.0f, Vector3f::UnitY()));
}

void painter3d::rotate(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

void painter3d::stop_rotating()
{
	view_rotation = move_rotation * view_rotation;
	move_rotation = Affine3f(AngleAxisf(0.0f, Vector3f::UnitX()));
}

void painter3d::scale(float wheel)
{
	scale3 += wheel / 10.0f;
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
	vertex_index = 0;
}

void render_3dpaint(const ImDrawList* parent_list, const ImDrawCmd* draw_command)
{
	if(parent_list) {}
	painter3d* painter = (painter3d *)draw_command->UserCallbackData;
	ImRect canvas = painter->get_window()->get_content_rectangle();
	gltool::state last_state; last_state.save_current_state();

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	ImVec2 viewport_position = ImVec2(canvas.Min.x,
									  ImGui::GetIO().DisplaySize.y - canvas.Max.y);
	glViewport((GLsizei)viewport_position.x, (GLsizei)viewport_position.y,
		(GLsizei)canvas.GetWidth(), (GLsizei)canvas.GetHeight());

	gltool::program program = *painter->get_program();
	program.use();
	glBindVertexArray(painter->get_vertex_array());
	glBindBuffer(GL_ARRAY_BUFFER, painter->get_vertex_buffer());
	glBufferData(GL_ARRAY_BUFFER, painter->get_vertex_index(), painter->get_vertices(), GL_STREAM_DRAW);

	GLuint attribute = program.get_attribute_location("position");
	program.enable_attribute_array(attribute);
	program.set_attribute_float_pointer(attribute);

	unsigned int vertices_number = painter->get_vertex_index();
	unsigned int count = GL_MAX_ELEMENTS_VERTICES;
	for(unsigned int start = 0; start < vertices_number; start += count)
	{
		if(start + count > vertices_number) count = vertices_number - start;
		glDrawArrays(GL_POINTS, start, count);
	}

	program.disable_attribute_array(attribute);
	last_state.restore();
}

void painter3d::draw()
{
	ImGui::GetWindowDrawList()->AddCallback(render_3dpaint, this);
}

void painter3d::init_view()
{
	program.compile(); program.use();
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnable(GL_PROGRAM_POINT_SIZE);

	move_translation = Translation3f(0.0f, 0.0f, 0.0f);
	move_rotation = Affine3f(AngleAxisf(0.0f, Vector3f::UnitX()));
	view_translation = Translation3f(300.0f, 300.0f, 300.0f);
	view_rotation = Affine3f(AngleAxisf(radians(-20), Vector3f::UnitX()) *
							 AngleAxisf(radians(20), Vector3f::UnitY()));
}


void painter3d::init_scene()
{
	clear();
	draw_zero_cross();
	draw_axes();
}

}
