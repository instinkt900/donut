#include "donut_pch.h"
#if defined(USE_OPENGL)
#include "opengl_shadersection.h"

namespace donut::opengl {
    namespace {
        GLenum ToGL(ShaderSectionType type) {
            switch (type) {
                case ShaderSectionType::Vertex: return GL_VERTEX_SHADER;
                case ShaderSectionType::Fragment: return GL_FRAGMENT_SHADER;
            }
            return 0;
        }
    }

    std::shared_ptr<ShaderSection> ShaderSection::Create(ShaderSectionType type, char const* source) {
        int shader = glCreateShader(ToGL(type));
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        int success;
        char message[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, message);
            spdlog::error("Vertex shader compile failed. {}", message);
            return nullptr;
        } else {
            return std::shared_ptr<ShaderSection>(new ShaderSection(shader));
        }
    }
    int ShaderSection::GetId() const {
        return m_id;
    }

    ShaderSection::ShaderSection(int id)
        : m_id(id) { }
}

#endif
