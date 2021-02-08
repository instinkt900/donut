#include "donut_pch.h"
#include "opengl_vertexbuffer.h"

namespace donut::opengl {
    std::shared_ptr<VertexBuffer> VertexBuffer::Create(VertexLayout const& layout, void* data, unsigned int count) {
        unsigned int vertexArray = 0;
        glGenVertexArrays(1, &vertexArray);
        if (vertexArray != 0) {
            unsigned int vertexBuffer = 0;
            glBindVertexArray(vertexArray);
            glGenBuffers(1, &vertexBuffer);
            if (vertexBuffer == 0) {
                glDeleteVertexArrays(1, &vertexArray);
            } else {
                glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
                glBufferData(GL_ARRAY_BUFFER, layout.GetStride() * count, data, GL_STATIC_DRAW);
                layout.Apply();
                return std::shared_ptr<VertexBuffer>(new VertexBuffer(vertexBuffer, vertexArray));
            }
        }
        return nullptr;
    }
    VertexBuffer::~VertexBuffer() {
        glDeleteVertexArrays(1, &m_vertexArray);
        glDeleteBuffers(1, &m_vertexBuffer);
    }
    void VertexBuffer::Bind() const {
        glBindVertexArray(m_vertexArray);
    }
    void VertexBuffer::Unbind() const {
        glBindVertexArray(0);
    }

    VertexBuffer::VertexBuffer(unsigned int vertexBuffer, unsigned int vertexArray)
        : m_vertexBuffer(vertexBuffer), m_vertexArray(vertexArray) { }
}
