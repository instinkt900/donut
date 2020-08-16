#pragma once

#ifdef USE_OPENGL
#include "opengl/opengl_shader.h"
namespace donut {
    using ShaderImpl = opengl::ShaderImpl;
}
#else
#error TODO
#endif
