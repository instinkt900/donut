#pragma once

namespace donut {
	class Shader {
    public:
        Shader() = default;
        Shader(std::initializer_list<ShaderSection> const& sections);
        ~Shader() = default;

        Shader(Shader const&) = default;
        Shader& operator=(Shader const&) = default;

        bool Valid() const { return nullptr != m_data; }
        operator bool() const { return Valid(); }

        void Bind() const;
        void Unbind() const;

        void SetVector3(std::string const& name, glm::vec3 const& vec);
        void SetVector4(std::string const& name, glm::vec4 const& vec);
        void SetMatrix33(std::string const& name, glm::mat3x3 const& mat);
        void SetMatrix44(std::string const& name, glm::mat4x4 const& mat);

    private:
        struct Data {
            ~Data() { glDeleteProgram(m_id); }
            int m_id;
            std::unordered_map<std::string, int> m_uniformCache;
        };
        std::shared_ptr<Data> m_data;

        int GetUniformLoc(std::string const& name);
	};
}