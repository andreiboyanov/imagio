#include "pointcloud_painter.h"

#include <iostream>

namespace wimgui
{

void pointcloud_painter::clear()
{
}


void pointcloud_painter::gl_paint(view3d& view)
{
	if(auto vertices = vertex_pointer.lock())
	{
		ImRect canvas = view.get_content_rectangle();
		gltool::state state;
		state.save_current_state();
		state.activate_imgui_defaults();

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
		glBindBuffer(GL_ARRAY_BUFFER, get_vertex_buffer());

		GLuint position_attribute = program.get_attribute_location("position");
		//GLuint color_attribute = program.get_attribute_location("color");
		//GLuint size_attribute = program.get_attribute_location("size");
		program.enable_attribute_array(position_attribute);
		//program.enable_attribute_array(color_attribute);
		//program.enable_attribute_array(size_attribute);
		program.set_attribute_float_pointer(
			position_attribute, 3, sizeof(vertex),
			(GLvoid *)0
		);
		//program.set_attribute_float_pointer(
		//	color_attribute, 3, sizeof(vertex),
		//	(GLvoid *)(3 * sizeof(float))
		//);
		//program.set_attribute_float_pointer(
		//	size_attribute, 1, sizeof(vertex),
		//	(GLvoid *)(6 * sizeof(float))
		//);

		transformation_matrix = view.get_view_matrix() * model_matrix;
		program.set_uniform("view_matrix", glm::value_ptr(transformation_matrix));

		unsigned int vertices_count = vertices->size();
		unsigned int count = GL_MAX_ELEMENTS_VERTICES;
		glBufferData(
			GL_ARRAY_BUFFER,
			vertices_count * sizeof(float) * 3,
			vertices->data(),
			GL_STREAM_DRAW
		);
		for(unsigned int start = 0; start < vertices_count; start += count)
		{
			if(start + count > vertices_count) count = vertices_count - start;
			glDrawArrays(GL_POINTS, start, count);
		}

		program.disable_attribute_array(position_attribute);
		//program.disable_attribute_array(color_attribute);
		//program.disable_attribute_array(size_attribute);
		state.restore();
	}
}

void pointcloud_painter::init_painter()
{
	program.compile(); program.use();
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnable(GL_PROGRAM_POINT_SIZE);
	// model_matrix = glm::rotate(model_matrix, -30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}


}
