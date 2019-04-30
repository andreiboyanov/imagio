#include "mesh_painter.h"

#include <iostream>

namespace wimgui
{

void mesh_painter::clear()
{
}


void mesh_painter::gl_paint(view3d& view)
{
	ImRect canvas = view.get_content_rectangle();
	state gl_state;
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
	glBindVertexArray(get_vertex_array());
	transformation_matrix = view.get_view_matrix() * model_matrix;
	program.set_uniform("view_matrix", glm::value_ptr(transformation_matrix));

	glDrawArrays(GL_TRIANGLES, 0, imagio::meshes::juan2_01::vertice_count * 3);
	glBindVertexArray(0);

	gl_state.restore();
}


void mesh_painter::init_painter()
{
	program.compile(); program.use();

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * imagio::meshes::juan2_01::vertice_count * 3,
		imagio::meshes::juan2_01::vertices,
		GL_STATIC_DRAW
	);
	GLuint position_attribute = program.get_attribute_location("position");
	program.set_attribute_float_pointer(position_attribute, 3);
	program.enable_attribute_array(position_attribute);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * imagio::meshes::juan2_01::vertice_count * 3,
		imagio::meshes::juan2_01::normals,
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
	model_matrix = glm::scale(model_matrix, glm::vec3(0.3f));
}

}
