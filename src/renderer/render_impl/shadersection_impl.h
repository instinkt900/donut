#pragma once

// TODO find a better place for this
namespace donut {
    enum class ShaderSectionType {
        Vertex,
        Fragment
    };
}

#ifdef USE_OPENGL
#include "opengl/opengl_shadersection.h"
namespace donut {
    using ShaderSectionImpl = opengl::ShaderSectionImpl;
}
#else
#error TODO
#endif
