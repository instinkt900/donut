#pragma once

#include "render_impl/vertexbuffer_impl.h"

namespace donut {
    class VertexBuffer {
    public:
        VertexBuffer() = default;
        VertexBuffer(VertexLayout layout, void* data, unsigned int count);
        VertexBuffer(VertexBuffer const&) = default;
        VertexBuffer& operator=(VertexBuffer const&) = default;
        ~VertexBuffer() = default;

        bool Valid() const { return nullptr != m_impl; }
        operator bool() const { return Valid(); }

        void Bind() const;
        void Unbind() const;

    private:
        std::shared_ptr<VertexBufferImpl> m_impl;
    };
}