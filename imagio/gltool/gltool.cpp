#include "gltool.h"

#include <GLFW/glfw3.h>

//int main()
//{
    //if( !glfwInit() )
    //{
    //        fprintf( stderr, "Failed to initialize GLFW\n" );
    //            return -1;
    //}
    //glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 
    //
    //GLFWwindow* window; // (In the accompanying source code, this variable is global)
    //window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
    //if( window == NULL ){
    //    fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
    //        glfwTerminate();
    //            return -1;
    //            }
    //            glfwMakeContextCurrent(window); // Initialize GLEW
    //            glewExperimental=true; // Needed in core profile

    //GLenum error = glewInit();
    //if (error != GLEW_OK)
    //{
    //    printf("GLEW error %s\n", glewGetErrorString(error));
    //    exit((int)error);
    //}
    //gltool::program program;
    //GLuint id = program.compile();

    //printf("Program id = %d\n", (int)id);
    //printf("Vertex shader: \n%s", program.get_vertex_shader().c_str());
//    return 0;
//}
