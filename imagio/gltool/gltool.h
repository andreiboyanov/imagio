#ifndef IMAGIO_GLTOOL_H
#define IMAGIO_GLTOOL_H

#include <GL/glew.h>
#include <GL/glut.h>

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
            glShaderSource(vertex_shader_id, 1, &vertex_shader_pointer , NULL);
            glCompileShader(vertex_shader_id);
            // glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &check_result);
            // glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
            //
            fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
            const char * fragment_shader_pointer = fragment_shader.c_str();
            glShaderSource(fragment_shader_id, 1, &fragment_shader_pointer , NULL);
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

};
#endif
