#pragma once

namespace donut::opengl {
    class ShaderSectionImpl : public NonCopyable {
    public:
        static std::shared_ptr<ShaderSectionImpl> Create(ShaderSectionType type, char const* source);
        int GetId() const;

    private:
        int m_id;
        ShaderSectionImpl(int id);
    };

}