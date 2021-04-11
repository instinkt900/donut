#pragma once

#if defined(USE_OPENGL)
#include "renderer_apis/opengl/opengl_glfw_window.h"
namespace donut {
    using Window = opengl::GLFWWindow;
}
#elif defined(USE_VULKAN)
#include "renderer_apis/vulkan/vulkan_glfw_window.h"
namespace donut {
    using Window = vulkan::GLFWWindow;
}
#else
#error TODO
#endif
