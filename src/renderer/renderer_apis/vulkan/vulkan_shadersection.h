#pragma once
#if defined(USE_VULKAN)

namespace donut::vulkan {
    class ShaderSection : public NonCopyable {
    public:
        static std::shared_ptr<ShaderSection> Create(ShaderSectionType type, char const* source);
        int GetId() const;

    private:
        int m_id;
        ShaderSection(int id);
    };

}

#endif
