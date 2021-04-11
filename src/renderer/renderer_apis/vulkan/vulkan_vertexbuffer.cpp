#include "donut_pch.h"
#if defined(USE_VULKAN)

#include "vulkan_vertexbuffer.h"

namespace donut::vulkan {
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(VertexLayout const& layout, void* data, unsigned int count) {
        return nullptr;
    }

    VertexBuffer::~VertexBuffer() {
    }

    void VertexBuffer::Bind() const {
    }

    void VertexBuffer::Unbind() const {
    }

    VertexBuffer::VertexBuffer(unsigned int vertexBuffer, unsigned int vertexArray) { }
}

#endif
