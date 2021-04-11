#pragma once

#if defined(USE_OPENGL)
#include "renderer_apis/opengl/opengl_vertexbuffer.h"
namespace donut {
    using VertexBuffer = opengl::VertexBuffer;
}
#elif defined(USE_VULKAN)
#include "renderer_apis/vulkan/vulkan_vertexbuffer.h"
namespace donut {
    using VertexBuffer = vulkan::VertexBuffer;
}
#else
#error TODO
#endif
