#include "gizmo_painter.h"

#include <iostream>

namespace wimgui
{

void gizmo_painter::clear()
{
}


void gizmo_painter::gl_paint(view3d& view)
{
	ImRect canvas = view.get_content_rectangle();
	gl_program::state gl_state;
	gl_state.save_current_state();
	gl_state.activate_imgui_defaults();

	ImVec2 viewport_position = ImVec2(
		canvas.Min.x,
		ImGui::GetIO().DisplaySize.y - canvas.Max.y
	);
	glViewport(
		(GLsizei)viewport_position.x,
		(GLsizei)viewport_position.y,
		(GLsizei)canvas.GetWidth(),
		(GLsizei)canvas.GetHeight()
	);

	program.use();
	glBindVertexArray(line_array);
	transformation_matrix = view.get_view_matrix() * model_matrix;
	program.set_uniform("view_matrix", glm::value_ptr(transformation_matrix));

	glLineWidth(5.0f);
	glDrawArrays(GL_LINES, 0, line_count * vertices_per_line * 3);
	glBindVertexArray(0);

	gl_state.restore();
}


void gizmo_painter::init_painter()
{
	program.compile(); program.use();

	glGenVertexArrays(1, &line_array);
	glBindVertexArray(line_array);
	glGenBuffers(1, &line_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, line_buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * line_count * vertices_per_line * 3,
		lines,
		GL_STATIC_DRAW
	);
	GLuint position_attribute = program.get_attribute_location("position");
	program.set_attribute_float_pointer(position_attribute, 3);
	program.enable_attribute_array(position_attribute);

	glGenBuffers(1, &line_normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, line_normal_buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * line_count * vertices_per_line * 3,
		line_normals,
		GL_STATIC_DRAW
	);
	GLuint normal_attribute = program.get_attribute_location("normal");
	program.set_attribute_float_pointer(normal_attribute, 3);
	program.enable_attribute_array(normal_attribute);

	glBindVertexArray(0);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	model_matrix = glm::scale(model_matrix, glm::vec3(0.1f));
}

}
