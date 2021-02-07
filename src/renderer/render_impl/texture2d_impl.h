#pragma once

namespace donut {
    enum class TextureFormat {
        RGB,
        RGBA,
        BGR,
        BGRA,
    };
}

#ifdef USE_OPENGL
#include "opengl/opengl_texture2d.h"
namespace donut {
    using Texture2DImpl = opengl::Texture2DImpl;
}
#else
#error Todo
#endif
