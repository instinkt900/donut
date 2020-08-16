#pragma once

#include "vertexelement.h"

namespace donut::opengl {
    class VertexLayoutImpl : public NonCopyable {
    public:
        VertexLayoutImpl(std::initializer_list<VertexElement> elements);
        ~VertexLayoutImpl() = default;

        size_t GetStride() const { return m_stride; }

        void Apply() const;

    private:
        std::vector<VertexElement> m_elements;
        size_t m_stride;
    };
}
