#include "donut_pch.h"
#if defined(USE_VULKAN)

#include "vulkan_shadersection.h"

namespace donut::vulkan {
    std::shared_ptr<ShaderSection> ShaderSection::Create(ShaderSectionType type, char const* source) {
        return nullptr;
    }

    ShaderSection::ShaderSection(int id)
        : m_id(id) { }
}

#endif
