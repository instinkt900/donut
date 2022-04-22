#pragma once

namespace donut::opengl {
    class ShaderSection : public NonCopyable {
    public:
        static std::shared_ptr<ShaderSection> Create(ShaderSectionType type, char const* source);
        int GetId() const;

    private:
        int m_id;
        ShaderSection(int id);
    };

}