#pragma once

namespace donut::opengl {
    class Texture2D : public NonCopyable {
    public:
        static std::shared_ptr<Texture2D> Create(int width, int height, TextureFormat format);
        static std::shared_ptr<Texture2D> Create(int width, int height, TextureFormat format, ImageFormat srcFormat, void const* data);
        static std::shared_ptr<Texture2D> Create(Image const& imageData, TextureFormat format);
        static std::vector<std::shared_ptr<Texture2D>> Create(int count, int width, int height, TextureFormat format);
        ~Texture2D();

        void Bind() const;
        void Unbind() const;

        unsigned int GetId() const { return m_textureId; }

    private:
        unsigned int m_textureId;

        Texture2D(unsigned int textureId);
    };
}