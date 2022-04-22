#include "donut_pch.h"
#if defined(USE_VULKAN)

#include "vulkan_shader.h"

namespace donut::vulkan {
    std::shared_ptr<Shader> Shader::Create(std::initializer_list<ShaderSection const*> const& sections) {
        return nullptr;
    }

    Shader::~Shader() {
    }

    void Shader::Bind() const {
    }

    void Shader::Unbind() const {
    }

    void Shader::SetVector3(std::string const& name, glm::vec3 const& vec) {
    }

    void Shader::SetVector4(std::string const& name, glm::vec4 const& vec) {
    }

    void Shader::SetMatrix33(std::string const& name, glm::mat3x3 const& mat) {
    }

    void Shader::SetMatrix44(std::string const& name, glm::mat4x4 const& mat) {
    }

    Shader::Shader(int program)
        : m_id(program) { }

}

#endif
