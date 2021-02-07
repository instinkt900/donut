#pragma once

#include "render_impl/texture2d_impl.h"

namespace donut {
    class Texture2D {
    public:
        Texture2D() = default;
        Texture2D(Image const& imageData);
        Texture2D(Texture2D const&) = default;
        Texture2D& operator=(Texture2D const&) = default;
        ~Texture2D() = default;

        bool Valid() const { return m_impl != nullptr; }
        operator bool() const { return Valid(); }

        void Bind() const;
        void Unbind() const;

    private:
        std::shared_ptr<Texture2DImpl> m_impl;
    };
}
