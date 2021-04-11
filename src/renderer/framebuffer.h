#pragma once

namespace donut {
    enum class FrameBufferTextureFormat {
        Unknown,
        RGBA,
        Depth24Stencil8,
    };

    struct FrameBufferTextureDesc {
        FrameBufferTextureFormat m_format;
    };

    struct FrameBufferDesc {
        unsigned int m_width;
        unsigned int m_height;
        std::vector<FrameBufferTextureDesc> m_attachments;
    };
}

#if defined(USE_OPENGL)
#include "renderer_apis/opengl/opengl_framebuffer.h"
namespace donut {
    using FrameBuffer = opengl::FrameBuffer;
}
#elif defined(USE_VULKAN)
#include "renderer_apis/vulkan/vulkan_framebuffer.h"
namespace donut {
    using FrameBuffer = vulkan::FrameBuffer;
}
#else
#error TODO
#endif
