#include "donut_pch.h"
#include "vertexlayout.h"

namespace donut {
    namespace {
        size_t TypeSize(VertexElementType const& type) {
            switch (type) {
                case VertexElementType::Float: return sizeof(float);
                case VertexElementType::Int: return sizeof(int);
                case VertexElementType::Uint: return sizeof(unsigned int);
                case VertexElementType::Short: return sizeof(short);
            }
            return 0;
        }

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

    VertexLayout::VertexLayout(std::initializer_list<VertexElement> elements) {
        m_stride = 0;
        m_elements.reserve(elements.size());
        for (auto&& el : elements) {
            m_elements.emplace_back(el);
            m_stride += el.m_count * TypeSize(el.m_type);
        }
    }

    void VertexLayout::Apply() const {
        size_t offset = 0;
        for (unsigned int i = 0; i < m_elements.size(); ++i) {
            glVertexAttribPointer(i, m_elements[i].m_count, ToGL(m_elements[i].m_type), m_elements[i].m_normalized, static_cast<GLsizei>(m_stride), (void*)offset);
            glEnableVertexAttribArray(i);
            offset += m_elements[i].m_count * TypeSize(m_elements[i].m_type);
        }
    }
}