#pragma once

#include "renderer/vertexlayout.h"

namespace donut::opengl {
    class VertexBuffer : public NonCopyable {
    public:
        static std::shared_ptr<VertexBuffer> Create(VertexLayout const& layout, void* data, unsigned int count);
        ~VertexBuffer();
        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_vertexBuffer;
        unsigned int m_vertexArray;

        VertexBuffer(unsigned int vertexBuffer, unsigned int vertexArray);
    };
}