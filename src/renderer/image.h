#pragma once

#include "imagedata.h"

namespace donut {
    class Image {
    public:
        Image(int width, int height, ImageFormat format, void* data = nullptr);
        Image(std::string const& filename);
        Image(Image const& other) = default;
        Image& operator=(Image const& other) = default;
        ~Image() = default;

        bool Valid() const { return m_image != nullptr; }
        operator bool() const { return Valid(); }

        int GetWidth() const { return m_image->GetWidth(); }
        int GetHeight() const { return m_image->GetHeight(); }
        ImageFormat GetFormat() const { return m_image->GetFormat(); }
        unsigned char* GetData() { return m_image->GetData(); }
        unsigned char const* GetData() const { return m_image->GetData(); }

    private:
        std::shared_ptr<ImageData> m_image;
    };
}