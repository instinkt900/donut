#include "donut_pch.h"
#include "opengl_vertexlayout.h"

namespace donut::opengl {
    namespace {
        GLuint ToGL(VertexElementType const& type) {
            switch (type) {
                case VertexElementType::Float: return GL_FLOAT;
                case VertexElementType::Int: return GL_INT;
                case VertexElementType::Uint: return GL_UNSIGNED_INT;
                case VertexElementType::Short: return GL_SHORT;
            }
            return 0;
        }
    }

    VertexLayoutImpl::VertexLayoutImpl(std::initializer_list<VertexElement> elements) {
        m_stride = 0;
        m_elements.reserve(elements.size());
        for (auto&& el : elements) {
            m_elements.emplace_back(el);
            m_stride += el.m_count * VertexElementTypeSize(el.m_type);
        }
    }

    void VertexLayoutImpl::Apply() const {
        size_t offset = 0;
        for (unsigned int i = 0; i < m_elements.size(); ++i) {
            glVertexAttribPointer(i, m_elements[i].m_count, ToGL(m_elements[i].m_type), m_elements[i].m_normalized, static_cast<GLsizei>(m_stride), (void*)offset);
            glEnableVertexAttribArray(i);
            offset += m_elements[i].m_count * VertexElementTypeSize(m_elements[i].m_type);
        }
    }
}
