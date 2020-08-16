#include "donut_pch.h"
#include "vertexlayout.h"

namespace donut {
    VertexLayout::VertexLayout(std::initializer_list<VertexElement> elements) {
        m_impl = std::make_shared<VertexLayoutImpl>(elements);
    }

    size_t VertexLayout::GetStride() const {
        assert(Valid());
        return m_impl->GetStride();
    }

    void VertexLayout::Apply() const {
        assert(Valid());
        m_impl->Apply();
    }
}
