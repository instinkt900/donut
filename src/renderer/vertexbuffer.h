#pragma once

#ifdef USE_OPENGL
#include "renderer_apis/opengl/opengl_vertexbuffer.h"
namespace donut {
    using VertexBuffer = opengl::VertexBuffer;
}
#else
#error TODO
#endif
