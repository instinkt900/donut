#include "donut_pch.h"
#include "vertexbuffer.h"

namespace donut {
    VertexBuffer::VertexBuffer(VertexLayout layout, void* data, unsigned int count) {
        m_impl = VertexBufferImpl::Create(layout, data, count);
    }

    void VertexBuffer::Bind() const {
        assert(Valid());
        m_impl->Bind();
    }

    void VertexBuffer::Unbind() const {
        assert(Valid());
        m_impl->Unbind();
    }
}