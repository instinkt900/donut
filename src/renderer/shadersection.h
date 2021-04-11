#pragma once

namespace donut {
    enum class ShaderSectionType {
        Vertex,
        Fragment
    };
}

#if defined(USE_OPENGL)
#include "renderer_apis/opengl/opengl_shadersection.h"
namespace donut {
    using ShaderSection = opengl::ShaderSection;
}
#elif defined(USE_VULKAN)
#include "renderer_apis/vulkan/vulkan_shadersection.h"
namespace donut {
    using ShaderSection = vulkan::ShaderSection;
}
#else
#error TODO
#endif
