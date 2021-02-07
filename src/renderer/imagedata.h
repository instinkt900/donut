#pragma once

namespace donut {
    enum class ImageFormat {
        INVALID,
        RGB,
        RGBA,
        BGR,
        BGRA
    };

    class ImageData {
    public:
        ImageData(int width, int height, ImageFormat format, void* data = nullptr);
        ~ImageData();

        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }
        ImageFormat GetFormat() const { return m_format; }
        unsigned char* GetData() const { return m_data; }

        bool Valid() const { return m_data != nullptr; }
        operator bool() const { return Valid(); }

    private:
        unsigned char* m_data = nullptr;
        int m_width = 0;
        int m_height = 0;
        ImageFormat m_format = ImageFormat::INVALID;
    };
}