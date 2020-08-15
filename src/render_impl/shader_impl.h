#pragma once

#ifdef USE_OPENGL
#include "opengl/opengl_shader.h"
namespace donut {
    using namespace opengl;
}
#else
#error TODO
#endif
