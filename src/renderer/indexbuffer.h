#pragma once

#ifdef USE_OPENGL
#include "renderer_apis/opengl/opengl_indexbuffer.h"
namespace donut {
    using IndexBuffer = opengl::IndexBuffer;
}
#else
#error TODO
#endif
