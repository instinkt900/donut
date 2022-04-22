#pragma once

namespace donut {
    enum class ShaderSectionType {
        Vertex,
        Fragment
    };
}

#ifdef USE_OPENGL
#include "renderer_apis/opengl/opengl_shadersection.h"
namespace donut {
    using ShaderSection = opengl::ShaderSection;
}
#else
#error TODO
#endif
