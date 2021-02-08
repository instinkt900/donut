#include "donut_pch.h"
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace donut {
    namespace {
        int ChannelCount(ImageFormat format) {
            switch (format) {
                case ImageFormat::RGB:
                case ImageFormat::BGR:
                    return 3;
                case ImageFormat::RGBA:
                case ImageFormat::BGRA:
                    return 4;
            }
            return 1;
        }
    }

    std::shared_ptr<Image> Image::CreateFromFile(std::string const& filename) {
        int width, height, channels;
        auto const stbimage = stbi_load(filename.c_str(), &width, &height, &channels, 3);
        if (stbimage) {
            auto const format = channels <= 3 ? ImageFormat::RGB : ImageFormat::RGBA;
            return std::make_shared<Image>(width, height, format, stbimage);
        }
        return nullptr;
    }

    Image::Image(int width, int height, ImageFormat format, void* data)
        : m_width(width)
        , m_height(height)
        , m_format(format) {
        auto const bufferSize = static_cast<size_t>(width) * height * ChannelCount(format);
        m_data = new unsigned char[bufferSize];
        if (data)
            std::copy_n(static_cast<unsigned char*>(data), bufferSize, m_data);
        else
            std::fill_n(m_data, bufferSize, 0x0);
    }

    Image::~Image() {
        delete[] m_data;
    }
}
