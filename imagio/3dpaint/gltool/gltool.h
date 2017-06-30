#ifndef IMAGIO_GLTOOL_H
#define IMAGIO_GLTOOL_H

#include <GL/gl3w.h>

#include <stddef.h>
#include <string>
#include <vector>

namespace gltool
{

class program
{
private:
	GLuint vertex_shader_id;
	std::string vertex_shader = std::string(R"glsl(
		#version 330 core
		
		layout(location=0) in vec3 position;
		layout(location=1) in vec3 color;
		layout(location=2) in float size;

        out vec4 geometry_color;

		uniform mat4 view_matrix;
		
		void main()
		{
			gl_Position = view_matrix * vec4(position, 1.0);
			gl_PointSize = size;
			geometry_color = vec4(color, 1.0);
		}
	)glsl");

	GLuint geometry_shader_id;
	std::string geometry_shader = std::string(R"glsl(
		#version 330 core
		layout (points) in;
		layout (triangle_strip, max_vertices = 15) out;

		in vec4 geometry_color[];
		out vec4 fragment_color;

		uniform mat4 view_matrix;

		const float delta = 0.005;
		const vec4 cube_primitive_1[9] = vec4[9](
		    vec4(-delta, -delta, -delta, 1.0),  // fbl - 1
		    vec4(-delta, +delta, -delta, 1.0),  // ful - 2
		    vec4(+delta, +delta, -delta, 1.0),  // fur - 3
		    vec4(+delta, +delta, +delta, 1.0),  // bur - 4
		    vec4(+delta, -delta, +delta, 1.0),  // bbr - 5
		    vec4(-delta, +delta, +delta, 1.0),  // bul - 6
		    vec4(-delta, -delta, +delta, 1.0),  // bbl - 7
		    vec4(-delta, +delta, -delta, 1.0),  // ful - 8
		    vec4(-delta, -delta, -delta, 1.0)  // fbl - 9
		);
		const vec4 cube_primitive_2[6] = vec4[6](
		    vec4(-delta, -delta, +delta, 1.0),  // bbl - 1
		    vec4(-delta, -delta, -delta, 1.0),  // fbl - 2
		    vec4(+delta, -delta, +delta, 1.0),  // bbr - 3
		    vec4(+delta, -delta, -delta, 1.0),  // fbr - 4
		    vec4(+delta, +delta, -delta, 1.0),  // fur - 5
		    vec4(-delta, -delta, -delta, 1.0)  // fbl - 6
		);

		void main()
		{    
			fragment_color = geometry_color[0];
		    vec4 position = gl_in[0].gl_Position;
		    for(int i = 0; i < 9; i++)
		    {
		        gl_Position = position + view_matrix * cube_primitive_1[i];
		        EmitVertex();   
		    }
		    EndPrimitive();
		    for(int i = 0; i < 6; i++)
		    {
		        gl_Position = position + view_matrix * cube_primitive_2[i];
		        EmitVertex();   
		    }
		    EndPrimitive();
		}
	)glsl");

	GLuint fragment_shader_id;
	std::string fragment_shader = std::string(R"gsls(
		#version 330 core

		in vec4 fragment_color;
		out vec4 out_color;
		
		void main()
		{
		   out_color = fragment_color;
		}
	)gsls");
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

		geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
		const char * geometry_shader_pointer = geometry_shader.c_str();
		glShaderSource(geometry_shader_id, 1, &geometry_shader_pointer, NULL);
		glCompileShader(geometry_shader_id);
		glGetShaderiv(geometry_shader_id, GL_COMPILE_STATUS, &compilation_result);
		glGetShaderiv(geometry_shader_id, GL_INFO_LOG_LENGTH, &compilation_result_length);
		if(compilation_result_length > 0)
		{
			std::vector<char> error_message(compilation_result_length + 1);
			glGetShaderInfoLog(geometry_shader_id, compilation_result_length, NULL, &error_message[0]);
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
		glAttachShader(program_id, geometry_shader_id);
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
		glDetachShader(program_id, geometry_shader_id);
		glDetachShader(program_id, fragment_shader_id);

		glDeleteShader(vertex_shader_id);
		glDeleteShader(geometry_shader_id);
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

	void set_attribute_float_pointer(const char* attribute_name, int size=3, GLsizei stride = 0, const GLvoid* offset = 0)
	{
		set_attribute_float_pointer(get_attribute_location(attribute_name), size, stride, offset);
	}

	void set_attribute_float_pointer(GLuint attribute_position, int size=3, GLsizei stride = 0, const GLvoid* offset = 0)
	{
		glVertexAttribPointer(attribute_position, size, GL_FLOAT, GL_FALSE, stride, offset);
	}

	GLuint get_uniform_location(const char* attribute_name)
	{
		return glGetUniformLocation(program_id, attribute_name);
	}

	void set_uniform(const char* attribute_name, const GLfloat* value)
	{
		set_uniform(get_uniform_location(attribute_name), value);
	}

	void set_uniform(GLuint attribute_position, const GLfloat* value)
	{
		glUniformMatrix4fv(attribute_position, 1, GL_FALSE, value);
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
	}
};

};
#endif
