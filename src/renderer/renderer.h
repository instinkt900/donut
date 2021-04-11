#pragma once

#if defined(USE_OPENGL)
#include "renderer_apis/opengl/opengl_renderer.h"
namespace donut {
    using Renderer = opengl::Renderer;
}
#elif defined(USE_VULKAN)
#include "renderer_apis/vulkan/vulkan_renderer.h"
namespace donut {
    using Renderer = vulkan::Renderer;
}
#else
#error Todo
#endif
