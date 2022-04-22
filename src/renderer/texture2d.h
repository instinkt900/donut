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

#ifdef USE_OPENGL
#include "renderer_apis/opengl/opengl_texture2d.h"
namespace donut {
    using Texture2D = opengl::Texture2D;
}
#else
#error Todo
#endif
