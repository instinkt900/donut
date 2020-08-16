#pragma once

#ifdef USE_OPENGL
#include "opengl/opengl_vertexlayout.h"
namespace donut {
    using VertexLayoutImpl = opengl::VertexLayoutImpl;
}
#else
#error Todo
#endif
