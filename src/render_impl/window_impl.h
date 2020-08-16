#pragma once

#ifdef USE_OPENGL
#include "opengl/opengl_glfw_window.h"
namespace donut {
    using WindowImpl = opengl::GLFWWindowImpl;
}
#else
#error TODO
#endif
