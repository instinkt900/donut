#pragma once

#ifdef USE_OPENGL
#include "render_impl/opengl/opengl_glfw_window.h"
namespace donut {
    using Window = opengl::GLFWWindow;
}
#else
#error TODO
#endif
