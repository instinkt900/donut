#pragma once

#ifdef USE_OPENGL
#include "render_impl/opengl/opengl_renderer.h"
namespace donut {
    using Renderer = opengl::Renderer;
}
#else
#error Todo
#endif
