#pragma once

namespace donut::opengl {
    class Texture2DImpl : public NonCopyable {
    public:
        static std::shared_ptr<Texture2DImpl> Create(int width, int height, ImageFormat format, void const* data);
        ~Texture2DImpl();
        void Bind() const;
        void Unbind() const;

    private:
        unsigned int m_textureId;

        Texture2DImpl(unsigned int textureId);
    };
}