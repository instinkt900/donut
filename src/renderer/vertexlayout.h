#pragma once

#include "render_impl/vertexlayout_impl.h"

namespace donut {
	class VertexLayout {
    public:
        VertexLayout() = default;
        VertexLayout(std::initializer_list<VertexElement> elements);
        VertexLayout(VertexLayout const&) = default;
        VertexLayout& operator=(VertexLayout const&) = default;
        ~VertexLayout() = default;

        bool Valid() const { return m_impl != nullptr; }
        operator bool() const { return Valid(); }

        size_t GetStride() const;
        void Apply() const;

    private:
        std::shared_ptr<VertexLayoutImpl> m_impl;
	};
}