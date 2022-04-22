#include "donut_pch.h"
#include "opengl_texture2d.h"

namespace donut::opengl {
    namespace {
        GLenum ToGL(ImageFormat type) {
            switch (type) {
                case ImageFormat::RGB: return GL_RGB;
                case ImageFormat::BGR: return GL_BGR;
                case ImageFormat::RGBA: return GL_RGBA;
                case ImageFormat::BGRA: return GL_BGRA;
            }
            return 0;
        }

        GLenum ToGL(TextureFormat format) {
            switch (format) {
                case TextureFormat::BGR: return GL_BGR;
                case TextureFormat::BGRA: return GL_BGRA;
                case TextureFormat::RGB: return GL_RGB8;
                case TextureFormat::RGBA: return GL_RGBA8;
                case TextureFormat::Depth: return GL_DEPTH24_STENCIL8;
            }
            return 0;
        }
    }

    std::shared_ptr<Texture2D> Texture2D::Create(int width, int height, TextureFormat format) {
        return Create(width, height, format, ImageFormat::RGB, nullptr);
    }

    std::shared_ptr<Texture2D> Texture2D::Create(int width, int height, TextureFormat format, ImageFormat srcFormat, void const* data) {
        unsigned int textureId = 0;
        glGenTextures(1, &textureId);
        if (textureId != 0) {
            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexStorage2D(GL_TEXTURE_2D, 1, ToGL(format), width, height);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, ToGL(srcFormat), GL_UNSIGNED_BYTE, data);
            return std::shared_ptr<Texture2D>(new Texture2D(textureId));
        }
        return nullptr;
    }

    std::shared_ptr<Texture2D> Texture2D::Create(Image const& imageData, TextureFormat format) {
        return Create(imageData.GetWidth(), imageData.GetHeight(), format, imageData.GetFormat(), imageData.GetData());
    }

    std::vector<std::shared_ptr<Texture2D>> Texture2D::Create(int count, int width, int height, TextureFormat format) {
        std::vector<std::shared_ptr<Texture2D>> textures(count);
        std::vector<unsigned int> textureIds(count);
        glCreateTextures(GL_TEXTURE_2D, static_cast<int>(textureIds.size()), textureIds.data());
        for (int i = 0; i < textureIds.size();  ++i) {
            if (textureIds[i] != 0) {
                glBindTexture(GL_TEXTURE_2D, textureIds[i]);
                glTexStorage2D(GL_TEXTURE_2D, 1, ToGL(format), width, height);
                textures[i] = std::shared_ptr<Texture2D>(new Texture2D(textureIds[i]));
            }
        }
        return textures;
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &m_textureId);
    }

    void Texture2D::Bind() const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
    }

    void Texture2D::Unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture2D::Texture2D(unsigned int textureId)
        : m_textureId(textureId) { }
}
