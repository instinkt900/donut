#include "donut_pch.h"
#include "shader.h"

namespace donut {
    Shader::Shader(std::initializer_list<ShaderSection> const& sections) {
        m_impl = ShaderImpl::Create(sections);
    }

    void Shader::Bind() const {
        assert(Valid());
        m_impl->Bind();
    }

    void Shader::Unbind() const {
        assert(Valid());
        m_impl->Unbind();
    }

    void Shader::SetVector3(std::string const& name, glm::vec3 const& vec) {
        assert(Valid());
        m_impl->SetVector3(name, vec);
    }

    void Shader::SetVector4(std::string const& name, glm::vec4 const& vec) {
        assert(Valid());
        m_impl->SetVector4(name, vec);
    }

    void Shader::SetMatrix33(std::string const& name, glm::mat3x3 const& mat) {
        assert(Valid());
        m_impl->SetMatrix33(name, mat);
    }

    void Shader::SetMatrix44(std::string const& name, glm::mat4x4 const& mat) {
        assert(Valid());
        m_impl->SetMatrix44(name, mat);
    }


}
