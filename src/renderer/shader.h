#pragma once

#if defined(USE_OPENGL)
#include "renderer_apis/opengl/opengl_shader.h"
namespace donut {
    using Shader = opengl::Shader;
}
#elif defined(USE_VULKAN)
#include "renderer_apis/vulkan/vulkan_shader.h"
namespace donut {
    using Shader = vulkan::Shader;
}
#else
#error TODO
#endif
