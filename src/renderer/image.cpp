#include "donut_pch.h"
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace donut {
    Image::Image(int width, int height, ImageFormat format, void* data) {
        m_image = std::make_shared<ImageData>(width, height, format, data);
    }

    Image::Image(std::string const& filename) {
        int width, height, channels;
        auto const stbimage = stbi_load(filename.c_str(), &width, &height, &channels, 3);
        if (stbimage) {
            auto const format = channels <= 3 ? ImageFormat::RGB : ImageFormat::RGBA;
            m_image = std::make_shared<ImageData>(width, height, format, stbimage);
        }
    }
}
