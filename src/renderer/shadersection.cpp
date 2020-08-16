#include "donut_pch.h"
#include "shadersection.h"

namespace donut {
    ShaderSection::ShaderSection(ShaderSectionType type, char const* source) {
        m_impl = ShaderSectionImpl::Create(type, source);
    }

    int ShaderSection::GetId() const {
        assert(Valid());
        return m_impl->GetId();
    }
}
