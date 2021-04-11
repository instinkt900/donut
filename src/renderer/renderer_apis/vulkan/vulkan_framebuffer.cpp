#include "donut_pch.h"
#if defined(USE_VULKAN)

#include "vulkan_framebuffer.h"

namespace donut::vulkan {
    std::shared_ptr<FrameBuffer> FrameBuffer::Create(FrameBufferDesc const& desc) {
        return std::make_shared<FrameBuffer>(desc);
    }

    FrameBuffer::FrameBuffer(FrameBufferDesc const& desc) {
    }

    FrameBuffer::~FrameBuffer() {
    }

    void FrameBuffer::Bind() {
    }

    void FrameBuffer::Unbind() {
    }

    std::shared_ptr<Texture2D> FrameBuffer::GetColorTexture(unsigned int index) const {
        return {};
    }
}

#endif
