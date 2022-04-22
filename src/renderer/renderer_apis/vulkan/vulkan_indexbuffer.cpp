#include "donut_pch.h"
#if defined(USE_VULKAN)

#include "vulkan_indexbuffer.h"

namespace donut::vulkan {
    std::shared_ptr<IndexBuffer> IndexBuffer::Create(unsigned int* data, unsigned int count) {
        return nullptr;
    }

    IndexBuffer::~IndexBuffer() {
    }

    unsigned int IndexBuffer::GetCount() const {
        return m_count;
    }

    void IndexBuffer::Bind() const {
    }

    void IndexBuffer::Unbind() const {
    }

    IndexBuffer::IndexBuffer(unsigned int id, unsigned int count)
        : m_count(count) { }
}

#endif
