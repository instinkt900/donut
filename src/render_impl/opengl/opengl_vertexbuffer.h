#pragma once

#include "vertexlayout.h"

namespace donut::opengl {
    class VertexBufferImpl {
    public:
        static std::shared_ptr<VertexBufferImpl> Create(VertexLayout layout, void* data, unsigned int count);
        ~VertexBufferImpl();
        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_vertexBuffer;
        unsigned int m_vertexArray;

        VertexBufferImpl(unsigned int vertexBuffer, unsigned int vertexArray);
    };
}