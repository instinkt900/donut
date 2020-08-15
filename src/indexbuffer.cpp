#include "donut_pch.h"
#include "indexbuffer.h"

namespace donut {
    IndexBuffer::IndexBuffer(unsigned int* data, unsigned int count) {
        m_impl = IndexBufferImpl::Create(data, count);
    }

    unsigned int IndexBuffer::GetCount() const {
        assert(Valid());
        return m_impl->GetCount();
    }

    void IndexBuffer::Bind() const {
        assert(Valid());
        m_impl->Bind();
    }

    void IndexBuffer::Unbind() const {
        assert(Valid());
        m_impl->Unbind();
    }
}
