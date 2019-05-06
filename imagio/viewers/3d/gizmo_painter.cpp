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

	lines_program.use();
	glBindVertexArray(line_array);
	transformation_matrix = view.get_view_matrix() * model_matrix;
	lines_program.set_uniform("view_matrix", glm::value_ptr(transformation_matrix));

	glLineWidth(5.0f);
	glDrawArrays(GL_LINES, 0, line_count * vertices_per_line * 3);
	glBindVertexArray(0);

	cones_program.use();
	glBindVertexArray(cones_array);
	transformation_matrix = view.get_view_matrix() * model_matrix;
	cones_program.set_uniform("view_matrix", glm::value_ptr(transformation_matrix));

	glBindBuffer(GL_ARRAY_BUFFER, x_cone_buffer);
	cones_program.set_uniform("color", glm::value_ptr(x_color));
	glDrawArrays(GL_TRIANGLES, 0, imagio::meshes::x_cone::vertice_count * 3);

	glBindBuffer(GL_ARRAY_BUFFER, y_cone_buffer);
	cones_program.set_uniform("color", glm::value_ptr(y_color));
	glDrawArrays(GL_TRIANGLES, 0, imagio::meshes::y_cone::vertice_count * 3);

	glBindBuffer(GL_ARRAY_BUFFER, z_cone_buffer);
	cones_program.set_uniform("color", glm::value_ptr(z_color));
	glDrawArrays(GL_TRIANGLES, 0, imagio::meshes::z_cone::vertice_count * 3);

	glBindVertexArray(0);


	gl_state.restore();
}


void gizmo_painter::init_painter()
{
	std::string vertex_shader_path("imagio/viewers/3d/gltool/per_vertex_color.vert");
	lines_program.load_vertex_shader(vertex_shader_path);
	lines_program.compile(); lines_program.use();

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
	GLuint position_attribute = lines_program.get_attribute_location("position");
	lines_program.set_attribute_float_pointer(position_attribute, 3);
	lines_program.enable_attribute_array(position_attribute);

	glGenBuffers(1, &line_normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, line_normal_buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * line_count * vertices_per_line * 3,
		line_normals,
		GL_STATIC_DRAW
	);
	GLuint normal_attribute = lines_program.get_attribute_location("normal");
	lines_program.set_attribute_float_pointer(normal_attribute, 3);
	lines_program.enable_attribute_array(normal_attribute);

	glGenBuffers(1, &line_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, line_color_buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * line_count * vertices_per_line * 4,
		line_colors,
		GL_STATIC_DRAW
	);
	GLuint color_attribute = lines_program.get_attribute_location("color");
	lines_program.set_attribute_float_pointer(color_attribute, 4);
	lines_program.enable_attribute_array(color_attribute);

	glBindVertexArray(0);


	std::string cones_vertex_shader_path("imagio/viewers/3d/gltool/single_color.vert");
	cones_program.load_vertex_shader(vertex_shader_path);
	cones_program.compile(); cones_program.use();

	glGenVertexArrays(1, &cones_array);
	glBindVertexArray(cones_array);
	bind_mesh_data(
		cones_program,
		imagio::meshes::x_cone::vertice_count,
		&x_cone_buffer,
		imagio::meshes::x_cone::vertices,
		&x_cone_normal_buffer,
		imagio::meshes::x_cone::normals
	);
	bind_mesh_data(
		cones_program,
		imagio::meshes::y_cone::vertice_count,
		&y_cone_buffer,
		imagio::meshes::y_cone::vertices,
		&y_cone_normal_buffer,
		imagio::meshes::y_cone::normals
	);
	bind_mesh_data(
		cones_program,
		imagio::meshes::z_cone::vertice_count,
		&z_cone_buffer,
		imagio::meshes::z_cone::vertices,
		&z_cone_normal_buffer,
		imagio::meshes::z_cone::normals
	);

	glBindVertexArray(0);

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	model_matrix = glm::scale(model_matrix, glm::vec3(0.1f));
}

void gizmo_painter::bind_mesh_data(
	gl_program::gl_program& program,
	int vertice_count,
	GLuint* vertex_buffer_ptr,
	float const* vertices_ptr,
	GLuint* normal_buffer_ptr,
	float const* normals_ptr
)
{
	glGenBuffers(1, vertex_buffer_ptr);
	glBindBuffer(GL_ARRAY_BUFFER, *vertex_buffer_ptr);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * vertice_count * 3,
		vertices_ptr,
		GL_STATIC_DRAW
	);
	GLuint position_attribute = program.get_attribute_location("position");
	program.set_attribute_float_pointer(position_attribute, 3);
	program.enable_attribute_array(position_attribute);

	glGenBuffers(1, normal_buffer_ptr);
	glBindBuffer(GL_ARRAY_BUFFER, *normal_buffer_ptr);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * vertice_count * 3,
		normals_ptr,
		GL_STATIC_DRAW
	);
	GLuint normal_attribute = program.get_attribute_location("normal");
	program.set_attribute_float_pointer(normal_attribute, 3);
	program.enable_attribute_array(normal_attribute);

}

}
