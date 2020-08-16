#include "donut_pch.h"
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

    std::shared_ptr<ShaderSectionImpl> ShaderSectionImpl::Create(ShaderSectionType type, char const* source) {
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
            return std::shared_ptr<ShaderSectionImpl>(new ShaderSectionImpl(shader));
        }
    }
    int ShaderSectionImpl::GetId() const {
        return m_id;
    }

    ShaderSectionImpl::ShaderSectionImpl(int id)
        : m_id(id) { }
}
