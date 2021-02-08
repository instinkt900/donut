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
    }

    std::shared_ptr<Texture2D> Texture2D::Create(int width, int height, ImageFormat format, void const* data) {
        unsigned int textureId = 0;
        glGenTextures(1, &textureId);
        if (textureId != 0) {
            glBindTexture(GL_TEXTURE_2D, textureId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, ToGL(format), GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            return std::shared_ptr<Texture2D>(new Texture2D(textureId));
        }
        return nullptr;
    }

    std::shared_ptr<Texture2D> Texture2D::Create(Image const& imageData) {
        return Create(imageData.GetWidth(), imageData.GetHeight(), imageData.GetFormat(), imageData.GetData());
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
