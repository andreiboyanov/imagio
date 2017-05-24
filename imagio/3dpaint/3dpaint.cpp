#include "3dpaint.h"

#include <iostream>

namespace wimgui
{

void painter3d::draw_point(float x, float y, float z, ImColor& color, float vertex_size)
{
	if(vertex_index + sizeof(vertex) > get_max_bytes()) { return; }
	vertices[vertex_index].position_x = x;
	vertices[vertex_index].position_y = y;
	vertices[vertex_index].position_z = z;
    vertices[vertex_index].color_r = color.Value.x;
    vertices[vertex_index].color_g = color.Value.y;
    vertices[vertex_index].color_b = color.Value.z;
    vertices[vertex_index].size = vertex_size;
    vertex_index++;
}


void painter3d::draw_zero_cross()
{
}

void painter3d::draw_axes()
{
}

void painter3d::move(float x, float y)
{
	if(x > 0) {}
	if(y > 0) {}
}

void painter3d::move(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

void painter3d::stop_moving()
{
}

void painter3d::rotate(float x, float y)
{
	if(x > 0) {}
	if(y > 0) {}

	glm::mat4 x_rotation, y_rotation;
	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	x_rotation = glm::rotate(x_rotation, radians(x), y_axis);
	y_rotation = glm::rotate(y_rotation, radians(y), x_axis);
	view_matrix = temporary_view_matrix * x_rotation * y_rotation;
}

void painter3d::rotate(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

void painter3d::stop_rotating()
{
	temporary_view_matrix = view_matrix;
}

void painter3d::scale(float wheel)
{
	if(wheel > 1.0f) {}
}

void painter3d::set_view_rotation(float x, float y, float z)
{
	glm::mat4 x_rotation, y_rotation, z_rotation;
	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
	x_rotation = glm::rotate(x_rotation, x, x_axis);
	y_rotation = glm::rotate(y_rotation, y, y_axis);
	z_rotation = glm::rotate(z_rotation, z, z_axis);
	view_matrix = x_rotation * y_rotation * z_rotation;
	stop_rotating();
}

void painter3d::set_view_translation(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
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
	gltool::state state; state.save_current_state();
	state.activate_imgui_defaults();

	ImVec2 viewport_position = ImVec2(canvas.Min.x,
									  ImGui::GetIO().DisplaySize.y - canvas.Max.y);
	glViewport((GLsizei)viewport_position.x, (GLsizei)viewport_position.y,
		(GLsizei)canvas.GetWidth(), (GLsizei)canvas.GetHeight());

	gltool::program program = *painter->get_program();
	program.use();
	glBindVertexArray(painter->get_vertex_array());
	glBindBuffer(GL_ARRAY_BUFFER, painter->get_vertex_buffer());

	GLuint position_attribute = program.get_attribute_location("position");
	GLuint color_attribute = program.get_attribute_location("color");
	GLuint size_attribute = program.get_attribute_location("size");
	program.enable_attribute_array(position_attribute);
	program.enable_attribute_array(color_attribute);
	program.enable_attribute_array(size_attribute);
	program.set_attribute_float_pointer(position_attribute, 3, sizeof(vertex), (GLvoid *)0);
	program.set_attribute_float_pointer(color_attribute, 3, sizeof(vertex), (GLvoid *)(3 * sizeof(float)));
	program.set_attribute_float_pointer(size_attribute, 1, sizeof(vertex), (GLvoid *)(6 * sizeof(float)));
	program.set_uniform("view_matrix", painter->get_transformation_pointer());

	unsigned int vertices_count = painter->get_vertex_index();
	unsigned int count = GL_MAX_ELEMENTS_VERTICES;
	glBufferData(GL_ARRAY_BUFFER, vertices_count * sizeof(vertex), painter->get_vertices(), GL_STREAM_DRAW);
	for(unsigned int start = 0; start < vertices_count; start += count)
	{
		if(start + count > vertices_count) count = vertices_count - start;
		glDrawArrays(GL_POINTS, start, count);
	}

	program.disable_attribute_array(position_attribute);
	program.disable_attribute_array(color_attribute);
	program.disable_attribute_array(size_attribute);
	state.restore();
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
	// FIXME: Check if the projection correction is OK
	projection_matrix = glm::ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, -2.0f, 2.0f);
	// model_matrix = glm::rotate(model_matrix, -30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}


void painter3d::init_scene()
{
	clear();
	draw_zero_cross();
	draw_axes();
}

}
