#pragma once

namespace donut {
    enum class ShaderSectionType {
        Vertex,
        Fragment
    };
}

#ifdef USE_OPENGL
#include "opengl/opengl_shadersection.h"
namespace donut {
    using namespace opengl;
}
#else
#error TODO
#endif
