#include "donut_pch.h"
#include "opengl_shader.h"

namespace donut::opengl {
    std::shared_ptr<Shader> Shader::Create(std::initializer_list<ShaderSection const*> const& sections) {
        int program = glCreateProgram();

        for (auto&& section : sections)
            glAttachShader(program, section->GetId());
        glLinkProgram(program);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char message[512];
            glGetProgramInfoLog(program, 512, nullptr, message);
            spdlog::error("Shader program link failed. {}", message);
            return nullptr;
        } else {
            return std::shared_ptr<Shader>(new Shader(program));
        }
    }

    Shader::~Shader() {
        glDeleteProgram(m_id);
    }

    void Shader::Bind() const {
        glUseProgram(m_id);
    }

    void Shader::Unbind() const {
        glUseProgram(0);
    }

    void Shader::SetVector3(std::string const& name, glm::vec3 const& vec) {
        glUniform3fv(GetUniformLoc(name), 1, (GLfloat*)&vec);
    }

    void Shader::SetVector4(std::string const& name, glm::vec4 const& vec) {
        glUniform4fv(GetUniformLoc(name), 1, (GLfloat*)&vec);
    }

    void Shader::SetMatrix33(std::string const& name, glm::mat3x3 const& mat) {
        glUniformMatrix3fv(GetUniformLoc(name), 1, GL_FALSE, (GLfloat*)&mat);
    }

    void Shader::SetMatrix44(std::string const& name, glm::mat4x4 const& mat) {
        glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, (GLfloat*)&mat);
    }

    Shader::Shader(int program)
        : m_id(program) { }

    int Shader::GetUniformLoc(std::string const& name) {
        auto const it = m_uniformCache.find(name);
        if (std::end(m_uniformCache) != it)
            return it->second;
        auto const loc = glGetUniformLocation(m_id, name.c_str());
        m_uniformCache[name] = loc;
#ifndef NDEBUG
        if (loc == -1)
            spdlog::error("Unable to find shader uniform \"{}\"", name);
#endif
        return loc;
    }
}
