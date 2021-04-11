#pragma once

#if defined(USE_OPENGL)
#include "renderer_apis/opengl/opengl_indexbuffer.h"
namespace donut {
    using IndexBuffer = opengl::IndexBuffer;
}
#elif defined(USE_VULKAN)
#include "renderer_apis/vulkan/vulkan_indexbuffer.h"
namespace donut {
    using IndexBuffer = vulkan::IndexBuffer;
}
#else
#error TODO
#endif
