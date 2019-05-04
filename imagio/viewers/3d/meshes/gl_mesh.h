#pragma once

#ifndef IMAGIO_GL_MESH_H
#define IMAGIO_GL_MESH_H

#include <GL/gl3w.h>
#include <GL/gl.h>

#include <stddef.h>
#include <string>
#include <vector>

#include "viewers/3d/gltool/gl_program.h"

namespace wimgui
{

class mesh_program: public wimgui::gl_program::gl_program
{
public:
	mesh_program() {
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
					fragment_color = vec4(0.3, 0.3, 0.3, 1.0);
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

};
};
#endif
