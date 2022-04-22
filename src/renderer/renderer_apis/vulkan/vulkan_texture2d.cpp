#include "donut_pch.h"
#if defined(USE_VULKAN)

#include "vulkan_texture2d.h"

namespace donut::vulkan {
    std::shared_ptr<Texture2D> Texture2D::Create(int width, int height, TextureFormat format) {
        return nullptr;
    }

    std::shared_ptr<Texture2D> Texture2D::Create(int width, int height, TextureFormat format, ImageFormat srcFormat, void const* data) {
        return nullptr;
    }

    std::shared_ptr<Texture2D> Texture2D::Create(Image const& imageData, TextureFormat format) {
        return nullptr;
    }

    std::vector<std::shared_ptr<Texture2D>> Texture2D::Create(int count, int width, int height, TextureFormat format) {
        return {};
    }

    Texture2D::~Texture2D() {
    }

    void Texture2D::Bind() const {
    }

    void Texture2D::Unbind() const {
    }

    Texture2D::Texture2D(unsigned int textureId)
        : m_textureId(textureId) { }
}

#endif
