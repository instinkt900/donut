#pragma once

#ifdef USE_OPENGL
#include "renderer_apis/opengl/opengl_renderer.h"
namespace donut {
    using Renderer = opengl::Renderer;
}
#else
#error Todo
#endif
