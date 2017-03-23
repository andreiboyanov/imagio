#include "3dpaint.h"

#include <iostream>

namespace wimgui
{

void painter3d::draw_point(float x, float y, float z, ImColor& color)
{
	float scale = 5.0f;
	if(color.Value.x > 0) {}
	if(vertex_index + 3 > 3 * get_max_vertices()) { return; }
	ImRect canvas = window->get_content_rectangle();
	vertices[vertex_index++] = -0.5f + scale * x / (2.0f * canvas.GetWidth());
	vertices[vertex_index++] = 0.5f + scale * y / (2.0f * canvas.GetHeight());
	vertices[vertex_index++] = z - z - 0.5f;
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
}

void painter3d::rotate(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

void painter3d::stop_rotating()
{
}

void painter3d::scale(float wheel)
{
	if(wheel > 1.0f) {}
}

void painter3d::set_view_rotation(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
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
	program.enable_attribute_array(position_attribute);
	program.set_attribute_float_pointer(position_attribute);
	program.set_uniform("view_matrix", painter->get_transformation_pointer());

	unsigned int vertices_count = painter->get_vertex_index() / 3;
	unsigned int count = GL_MAX_ELEMENTS_VERTICES;
	glBufferData(GL_ARRAY_BUFFER, vertices_count * 3 * sizeof(float), painter->get_vertices(), GL_STREAM_DRAW);
	for(unsigned int start = 0; start < vertices_count; start += count)
	{
		if(start + count > vertices_count) count = vertices_count - start;
		glDrawArrays(GL_POINTS, start, count);
	}

	program.disable_attribute_array(position_attribute);
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
	projection_matrix = glm::ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	model_matrix = glm::rotate(model_matrix, -45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}


void painter3d::init_scene()
{
	clear();
	draw_zero_cross();
	draw_axes();
}

}
