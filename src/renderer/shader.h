#pragma once

#include "render_impl/shader_impl.h"

namespace donut {
	class Shader {
    public:
        Shader() = default;
        Shader(std::initializer_list<ShaderSection> const& sections);
        ~Shader() = default;

        Shader(Shader const&) = default;
        Shader& operator=(Shader const&) = default;

        bool Valid() const { return nullptr != m_impl; }
        operator bool() const { return Valid(); }

        void Bind() const;
        void Unbind() const;

        void SetVector3(std::string const& name, glm::vec3 const& vec);
        void SetVector4(std::string const& name, glm::vec4 const& vec);
        void SetMatrix33(std::string const& name, glm::mat3x3 const& mat);
        void SetMatrix44(std::string const& name, glm::mat4x4 const& mat);

    private:
        std::shared_ptr<ShaderImpl> m_impl;
	};
}