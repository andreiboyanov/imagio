#ifndef IMAGIO_GLTOOL_H
#define IMAGIO_GLTOOL_H

#include <GL/gl3w.h>

#include <stddef.h>
#include <string>

namespace gltool
{

class program
{
private:
	GLuint vertex_shader_id;
	std::string vertex_shader = std::string(""\
		"#version 330 core\n" \
		"\n" \
		"layout(location=0) in vec3 position;\n" \
		"\n" \
		"void main()\n" \
		"{\n" \
		"   glPosition.xyz = position;\n" \
		"   glPosition.w = 1.0;\n" \
		"}\n");
	GLuint fragment_shader_id;
	std::string fragment_shader = std::string(""\
		"#version 330 core\n" \
		"\n" \
		"out vec3 color\n" \
		"\n" \
		"void main()\n" \
		"{\n" \
		"   color = vec(1, 1, 1)\n" \
		"}\n");
	GLuint program_id;

public:
	program() {}

	GLuint get_id() { return program_id; }

	std::string get_vertex_shader_code() { return vertex_shader; }
	void set_vertex_shader_code(const char * new_vertex_shader)
	{
		vertex_shader = new_vertex_shader;
	}

	std::string get_framgent_shader_code() { return fragment_shader; }
	void set_fragment_shader_code(const char * new_fragment_shader)
	{
		fragment_shader = new_fragment_shader;
	}

	GLuint compile()
	{
		vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
		const char * vertex_shader_pointer = vertex_shader.c_str();
		glShaderSource(vertex_shader_id, 1, &vertex_shader_pointer, NULL);
		glCompileShader(vertex_shader_id);
		// glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &check_result);
		// glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		//
		fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		const char * fragment_shader_pointer = fragment_shader.c_str();
		glShaderSource(fragment_shader_id, 1, &fragment_shader_pointer, NULL);
		glCompileShader(fragment_shader_id);
		// glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &check_result);
		// glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

		program_id = glCreateProgram();
		glAttachShader(program_id, vertex_shader_id);
		glAttachShader(program_id, fragment_shader_id);
		glLinkProgram(program_id);

		// Check the program here

		glDetachShader(program_id, vertex_shader_id);
		glDetachShader(program_id, fragment_shader_id);

		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);

		return program_id;
	}
};

class state
{
private:
	GLint program;
	GLint texture;
	GLint active_texture;
	GLint array_buffer;
	GLint element_array_buffer;
	GLint vertex_array;
	GLint blend_src;
	GLint blend_dst;
	GLint blend_equation_rgb;
	GLint blend_equation_alpha;
	GLint viewport[4];
	GLint scissor_box[4];
	GLboolean enable_blend;
	GLboolean enable_cull_face;
	GLboolean enable_depth_test;
	GLboolean enable_scissor_test;
public:
	void save_current_state()
	{
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
		glGetIntegerv(GL_ACTIVE_TEXTURE, &active_texture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &array_buffer);
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &element_array_buffer);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertex_array);
		glGetIntegerv(GL_BLEND_SRC, &blend_src);
		glGetIntegerv(GL_BLEND_DST, &blend_dst);
		glGetIntegerv(GL_BLEND_EQUATION_RGB, &blend_equation_rgb);
		glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &blend_equation_alpha);
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetIntegerv(GL_SCISSOR_BOX, scissor_box);
		enable_blend = glIsEnabled(GL_BLEND);
		enable_cull_face = glIsEnabled(GL_CULL_FACE);
		enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
		enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
	}

	void restore()
	{
		glUseProgram(program);
		glActiveTexture(active_texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(vertex_array);
		glBindBuffer(GL_ARRAY_BUFFER, array_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer);
		glBlendEquationSeparate(blend_equation_rgb, blend_equation_alpha);
		glBlendFunc(blend_src, blend_dst);
		if (enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
		if (enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
		if (enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
		if (enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
		glViewport(viewport[0], viewport[1], (GLsizei)viewport[2], (GLsizei)viewport[3]);
		glScissor(scissor_box[0], scissor_box[1], (GLsizei)scissor_box[2], (GLsizei)scissor_box[3]);
	}
};

};
#endif
