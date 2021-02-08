#pragma once

#ifdef USE_OPENGL
#include "render_impl/opengl/opengl_vertexlayout.h"
namespace donut {
    using VertexLayout = opengl::VertexLayout;
}
#else
#error Todo
#endif
