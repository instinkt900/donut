#pragma once

#include "vertexelement.h"

namespace donut::opengl {
    class VertexLayout : public NonCopyable {
    public:
        VertexLayout(std::initializer_list<VertexElement> elements);
        ~VertexLayout() = default;

        size_t GetStride() const { return m_stride; }

        void Apply() const;

    private:
        std::vector<VertexElement> m_elements;
        size_t m_stride;
    };
}
