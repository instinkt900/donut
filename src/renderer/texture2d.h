#pragma once

namespace donut {
    enum class TextureFormat {
        RGB,
        RGBA,
        BGR,
        BGRA,
        Depth,
    };
}

#if defined(USE_OPENGL)
#include "renderer_apis/opengl/opengl_texture2d.h"
namespace donut {
    using Texture2D = opengl::Texture2D;
}
#elif defined(USE_VULKAN)
#include "renderer_apis/vulkan/vulkan_texture2d.h"
namespace donut {
    using Texture2D = vulkan::Texture2D;
}
#else
#error Todo
#endif
