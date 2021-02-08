#pragma once

#ifdef USE_OPENGL
#include "render_impl/opengl/opengl_shader.h"
namespace donut {
    using Shader = opengl::Shader;
}
#else
#error TODO
#endif
