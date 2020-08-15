#include "donut_pch.h"
#include "opengl_indexbuffer.h"

namespace donut::opengl {
    std::shared_ptr<IndexBufferImpl> IndexBufferImpl::Create(unsigned int* data, unsigned int count) {
        unsigned int id = 0;
        glGenBuffers(1, &id);
        if (id != 0) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW);
            return std::shared_ptr<IndexBufferImpl>(new IndexBufferImpl(id, count));
        }
        return nullptr;
    }

    IndexBufferImpl::~IndexBufferImpl() {
        glDeleteBuffers(1, &m_id);
    }
    unsigned int IndexBufferImpl::GetCount() const {
        return m_count;
    }
    void IndexBufferImpl::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }
    void IndexBufferImpl::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    IndexBufferImpl::IndexBufferImpl(unsigned int id, unsigned int count)
        : m_id(id), m_count(count) { }
}
