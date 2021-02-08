#pragma once

#ifdef USE_OPENGL
#include "renderer_apis/opengl/opengl_vertexlayout.h"
namespace donut {
    using VertexLayout = opengl::VertexLayout;
}
#else
#error Todo
#endif
