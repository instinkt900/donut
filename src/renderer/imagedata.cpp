#include "donut_pch.h"
#include "imagedata.h"

namespace donut {
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

    ImageData::ImageData(int width, int height, ImageFormat format, void* data)
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

    ImageData::~ImageData() {
        delete[] m_data;
    }
}
