#include "donut_pch.h"
#include "opengl_vertexbuffer.h"

namespace donut::opengl {
    std::shared_ptr<VertexBufferImpl> VertexBufferImpl::Create(VertexLayout layout, void* data, unsigned int count) {
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
                return std::shared_ptr<VertexBufferImpl>(new VertexBufferImpl(vertexBuffer, vertexArray));
            }
        }
        return nullptr;
    }
    VertexBufferImpl::~VertexBufferImpl() {
        glDeleteVertexArrays(1, &m_vertexArray);
        glDeleteBuffers(1, &m_vertexBuffer);
    }
    void VertexBufferImpl::Bind() const {
        glBindVertexArray(m_vertexArray);
    }
    void VertexBufferImpl::Unbind() const {
        glBindVertexArray(0);
    }

    VertexBufferImpl::VertexBufferImpl(unsigned int vertexBuffer, unsigned int vertexArray)
        : m_vertexBuffer(vertexBuffer), m_vertexArray(vertexArray) { }
}
