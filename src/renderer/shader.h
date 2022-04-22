#pragma once

#ifdef USE_OPENGL
#include "renderer_apis/opengl/opengl_shader.h"
namespace donut {
    using Shader = opengl::Shader;
}
#else
#error TODO
#endif
