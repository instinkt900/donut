#pragma once

#include "render_impl/shadersection_impl.h"

namespace donut {
	class ShaderSection {
    public:
        ShaderSection(ShaderSectionType type, char const* source);
        ShaderSection(ShaderSection const& other) = default;
        ShaderSection& operator=(ShaderSection const& other) = default;
        ~ShaderSection() = default;

        bool Valid() const { return nullptr != m_impl; }
        operator bool() const { return Valid(); }

        int GetId() const;

    private:
        std::shared_ptr<ShaderSectionImpl> m_impl;
	};
}