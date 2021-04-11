#pragma once

#if defined(USE_OPENGL)
#include "renderer_apis/opengl/opengl_vertexlayout.h"
namespace donut {
    using VertexLayout = opengl::VertexLayout;
}
#elif defined(USE_VULKAN)
#include "renderer_apis/vulkan/vulkan_vertexlayout.h"
namespace donut {
    using VertexLayout = vulkan::VertexLayout;
}
#else
#error Todo
#endif
