#version 330 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;

out vec4 fragment_color;
out vec3 fragment_position;
out vec3 fragment_normal;

uniform mat4 view_matrix;
uniform vec4 color;

void main()
{
    gl_Position = view_matrix * vec4(position, 1.0);
    fragment_color = color;
    fragment_position = vec3(gl_Position);
    fragment_normal = vec3(view_matrix * vec4(normal, 1.0));
}
