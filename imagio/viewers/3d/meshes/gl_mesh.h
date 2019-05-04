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
{};
};
#endif
