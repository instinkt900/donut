#pragma once

#include "render_impl/indexbuffer_impl.h"

namespace donut {
    class IndexBuffer {
    public:
        IndexBuffer() = default;
        IndexBuffer(unsigned int* data, unsigned int count);
        IndexBuffer(IndexBuffer const&) = default;
        IndexBuffer& operator=(IndexBuffer const&) = default;
        ~IndexBuffer() = default;

        bool Valid() const { return m_impl != nullptr; }
        operator bool() const { return Valid(); }

        unsigned int GetCount() const;
        void Bind() const;
        void Unbind() const;

    private:
        std::shared_ptr<IndexBufferImpl> m_impl;
    };
}