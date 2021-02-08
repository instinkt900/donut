#pragma once

#ifdef USE_OPENGL
#include "render_impl/opengl/opengl_indexbuffer.h"
namespace donut {
    using IndexBuffer = opengl::IndexBuffer;
}
#else
#error TODO
#endif
