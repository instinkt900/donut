#include "donut_pch.h"
#include "texture2d.h"

namespace donut {
    Texture2D::Texture2D(Image const& imageData) {
        if (imageData)
            m_impl = Texture2DImpl::Create(imageData.GetWidth(), imageData.GetHeight(), imageData.GetFormat(), imageData.GetData());
    }

    void Texture2D::Bind() const {
        assert(Valid());
        m_impl->Bind();
    }

    void Texture2D::Unbind() const {
        assert(Valid());
        m_impl->Unbind();
    }
}
