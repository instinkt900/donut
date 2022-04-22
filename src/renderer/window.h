#pragma once

#ifdef USE_OPENGL
#include "renderer_apis/opengl/opengl_glfw_window.h"
namespace donut {
    using Window = opengl::GLFWWindow;
}
#else
#error TODO
#endif
