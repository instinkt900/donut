#pragma once

namespace donut::opengl {
    class Texture2D : public NonCopyable {
    public:
        static std::shared_ptr<Texture2D> Create(int width, int height, ImageFormat format, void const* data);
        static std::shared_ptr<Texture2D> Create(Image const& imageData);
        ~Texture2D();

        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_textureId;

        Texture2D(unsigned int textureId);
    };
}