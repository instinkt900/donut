#pragma once

#ifdef USE_OPENGL
#include "opengl/opengl_vertexbuffer.h"
namespace donut {
    using VertexBufferImpl = opengl::VertexBufferImpl;
}
#else
#error TODO
#endif
