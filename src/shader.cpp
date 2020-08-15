#include "donut_pch.h"
#include "shader.h"

namespace donut {
    Shader::Shader(std::initializer_list<ShaderSection> const& sections) {
        int program = glCreateProgram();

        for (auto&& section : sections)
            glAttachShader(program, section.GetId());
        glLinkProgram(program);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char message[512];
            glGetProgramInfoLog(program, 512, nullptr, message);
            spdlog::error("Shader program link failed. {}", message);
        } else {
            m_data = std::make_shared<Data>();
            m_data->m_id = program;
        }
    }

    void Shader::Bind() const {
        assert(Valid());
        glUseProgram(m_data->m_id);
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

    int Shader::GetUniformLoc(std::string const& name) {
        assert(Valid());
        auto const it = m_data->m_uniformCache.find(name);
        if (std::end(m_data->m_uniformCache) != it)
            return it->second;

        auto const loc = glGetUniformLocation(m_data->m_id, name.c_str());
        m_data->m_uniformCache[name] = loc;

#ifndef NDEBUG
        if (loc == -1)
            spdlog::error("Unable to find shader uniform \"{}\"", name);
#endif

        return loc;
    }
}
