#pragma once

#ifndef IMAGIO_GL_PROGRAM_H
#define IMAGIO_GL_PROGRAM_H

#include <GL/gl3w.h>
#include <GL/gl.h>

#include <stddef.h>
#include <string>
#include <vector>

namespace wimgui
{

namespace gl_program
{


class gl_program
{
private:
	GLuint vertex_shader_id;
	std::string vertex_shader;

	GLuint fragment_shader_id;
	std::string fragment_shader;

	GLuint program_id;

public:
	gl_program() {
		set_vertex_shader_code(
			std::string(R"glsl(
				#version 330 core
				
				layout(location=0) in vec3 position;
				layout(location=1) in vec3 normal;

				out vec4 fragment_color;
				out vec3 fragment_position;
				out vec3 fragment_normal;

				uniform mat4 view_matrix;

				void main()
				{
					gl_Position = view_matrix * vec4(position, 1.0);
					fragment_color = vec4(0.4, 0.3, 0.6, 1.0);
					fragment_position = vec3(gl_Position);
					fragment_normal = vec3(view_matrix * vec4(normal, 1.0));
				}
			)glsl")
		);
		set_fragment_shader_code(
			std::string(R"gsls(
				#version 330 core

				in vec4 fragment_color;
				in vec3 fragment_position;
				in vec3 fragment_normal;

				uniform mat4 view_matrix;

				out vec4 out_color;

				uniform struct Light {
					vec3 position;
					vec3 color;
				};


				void main()
				{
					if (gl_FrontFacing == false) {
						return;
					}
					Light light = Light(vec3(0.0, 0.0, -2.0), vec3(1.0, 1.0, 1.0));
					float ambient_strength = 0.5;

					vec3 ambient = ambient_strength * light.color;

					vec3 normal = normalize(fragment_normal);
					vec3 light_direction = normalize(light.position - fragment_position);
					float diff = max(dot(normal, light_direction), 0);
					vec3 diffuse = diff * light.color;

					out_color = vec4((ambient + diffuse) * vec3(fragment_color), fragment_color[3]);
					// out_color = vec4(diffuse * vec3(fragment_color), fragment_color[3]);
				}
			)gsls")
		);
	}

	GLuint get_id() { return program_id; }

	std::string load_shader(std::string& path)
	{
		std::ifstream input_stream(path);
		std::string result(
			(std::istreambuf_iterator<char>(input_stream)),
            std::istreambuf_iterator<char>()
		);
		return result;
	}

	std::string get_vertex_shader_code() { return vertex_shader; }
	void set_vertex_shader_code(const std::string& new_vertex_shader)
	{
		vertex_shader = new_vertex_shader;
	}
	void load_vertex_shader(std::string& path)
	{
		vertex_shader = load_shader(path);
	}

	std::string get_fragment_shader_code() { return fragment_shader; }
	void set_fragment_shader_code(const std::string& new_fragment_shader)
	{
		fragment_shader = new_fragment_shader;
	}
	void load_fragment_shader(std::string& path)
	{
		vertex_shader = load_shader(path);
	}

	GLuint compile()
	{
		GLint compilation_result = GL_FALSE;
		int compilation_result_length;

		vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
		const char * vertex_shader_pointer = vertex_shader.c_str();
		glShaderSource(vertex_shader_id, 1, &vertex_shader_pointer, NULL);
		glCompileShader(vertex_shader_id);
		glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &compilation_result);
		glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &compilation_result_length);
		if(compilation_result_length > 0)
		{
			std::vector<char> error_message(compilation_result_length + 1);
			glGetShaderInfoLog(vertex_shader_id, compilation_result_length, NULL, &error_message[0]);
			printf("%s\n", &error_message[0]);
		}

		fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		const char * fragment_shader_pointer = fragment_shader.c_str();
		glShaderSource(fragment_shader_id, 1, &fragment_shader_pointer, NULL);
		glCompileShader(fragment_shader_id);
		glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &compilation_result);
		glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &compilation_result_length);
		if(compilation_result_length > 0)
		{
			std::vector<char> error_message(compilation_result_length + 1);
			glGetShaderInfoLog(fragment_shader_id, compilation_result_length, NULL, &error_message[0]);
			printf("%s\n", &error_message[0]);
		}

		program_id = glCreateProgram();
		glAttachShader(program_id, vertex_shader_id);
		glAttachShader(program_id, fragment_shader_id);
		glLinkProgram(program_id);

		glGetProgramiv(program_id, GL_LINK_STATUS, &compilation_result);
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &compilation_result_length);
		if(compilation_result_length > 0)
		{
			std::vector<char> error_message(compilation_result_length + 1);
			glGetProgramInfoLog(program_id, compilation_result_length, NULL, &error_message[0]);
			printf("%s\n", &error_message[0]);
		}

		glDetachShader(program_id, vertex_shader_id);
		glDetachShader(program_id, fragment_shader_id);

		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);

		return program_id;
	}

	void use()
	{
		glUseProgram(program_id);
	}

	GLuint get_attribute_location(const char* attribute_name)
	{
		return glGetAttribLocation(program_id, attribute_name);
	}

	void set_attribute_float_pointer(const char* attribute_name, int size=3, GLsizei stride=0, const GLvoid* offset=0)
	{
		set_attribute_float_pointer(get_attribute_location(attribute_name), size, stride, offset);
	}

	void set_attribute_float_pointer(GLuint attribute_position, int size=3, GLsizei stride=0, const GLvoid* offset=0)
	{
		glVertexAttribPointer(attribute_position, size, GL_FLOAT, GL_FALSE, stride, offset);
	}

	GLuint get_uniform_location(const char* uniform_name)
	{
		return glGetUniformLocation(program_id, uniform_name);
	}

	void set_uniform(const char* uniform_name, glm::mat4& value)
	{
		set_uniform(get_uniform_location(uniform_name), value);
	}

	void set_uniform(GLuint uniform_location, glm::mat4& value)
	{
		glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void set_uniform(const char* uniform_name, glm::vec4& value)
	{
		set_uniform(get_uniform_location(uniform_name), value);
	}

	void set_uniform(GLuint uniform_location, glm::vec4& value)
	{
		glUniform4fv(uniform_location, 1, glm::value_ptr(value));
	}

	void enable_attribute_array(GLuint attribute_position)
	{
		glEnableVertexAttribArray(attribute_position);
	}

	void disable_attribute_array(GLuint attribute_position)
	{
		glDisableVertexAttribArray(attribute_position);
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
		if(enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
		if(enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
		if(enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
		if(enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
		glViewport(viewport[0], viewport[1], (GLsizei)viewport[2], (GLsizei)viewport[3]);
		glScissor(scissor_box[0], scissor_box[1], (GLsizei)scissor_box[2], (GLsizei)scissor_box[3]);
	}

	void activate_imgui_defaults()
	{
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_ALPHA);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
};

};
};
#endif
