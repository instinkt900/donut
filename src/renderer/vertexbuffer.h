#pragma once

#ifdef USE_OPENGL
#include "render_impl/opengl/opengl_vertexbuffer.h"
namespace donut {
    using VertexBuffer = opengl::VertexBuffer;
}
#else
#error TODO
#endif
