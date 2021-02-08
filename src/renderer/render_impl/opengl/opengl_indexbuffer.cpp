#include "donut_pch.h"
#include "opengl_indexbuffer.h"

namespace donut::opengl {
    std::shared_ptr<IndexBuffer> IndexBuffer::Create(unsigned int* data, unsigned int count) {
        unsigned int id = 0;
        glGenBuffers(1, &id);
        if (id != 0) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW);
            return std::shared_ptr<IndexBuffer>(new IndexBuffer(id, count));
        }
        return nullptr;
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_id);
    }

    unsigned int IndexBuffer::GetCount() const {
        return m_count;
    }

    void IndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    IndexBuffer::IndexBuffer(unsigned int id, unsigned int count)
        : m_id(id), m_count(count) { }
}
