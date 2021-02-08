#pragma once

namespace donut {
    enum class ImageFormat {
        INVALID,
        RGB,
        RGBA,
        BGR,
        BGRA
    };

    class Image {
    public:
        static std::shared_ptr<Image> CreateFromFile(std::string const& filename);
        Image(int width, int height, ImageFormat format, void* data = nullptr);
        Image(Image const& other) = default;
        Image& operator=(Image const& other) = default;
        ~Image();

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        ImageFormat GetFormat() const { return m_format; }
        unsigned char* GetData() { return m_data; }
        unsigned char const* GetData() const { return m_data; }

    private:
        unsigned char* m_data = nullptr;
        int m_width = 0;
        int m_height = 0;
        ImageFormat m_format = ImageFormat::INVALID;
    };
}