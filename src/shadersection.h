#pragma once

namespace donut {
    enum class ShaderSectionType {
        Vertex,
        Fragment
    };

	class ShaderSection {
    public:
        ShaderSection(ShaderSectionType type, char const* source);
        ~ShaderSection() = default;

        ShaderSection(ShaderSection const& other) = default;
        ShaderSection& operator=(ShaderSection const& other) = default;

        bool Valid() const { return nullptr != m_data; }
        operator bool() const { return Valid(); }

        int GetId() const {
            assert(Valid());
            return m_data->m_id;
        }

    private:
        struct Data {
            ~Data() { glDeleteShader(m_id); }
            int m_id = 0;
        };

        std::shared_ptr<Data> m_data;
	};
}