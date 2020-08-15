#include "donut_pch.h"
#include "shadersection.h"

namespace donut {
    namespace {
        GLenum ToGL(ShaderSectionType type) {
            switch (type) {
                case ShaderSectionType::Vertex: return GL_VERTEX_SHADER;
                case ShaderSectionType::Fragment: return GL_FRAGMENT_SHADER;
            }
            return 0;
        }
    }
    ShaderSection::ShaderSection(ShaderSectionType type, char const* source) {
        int vertexShader = glCreateShader(ToGL(type));
        glShaderSource(vertexShader, 1, &source, nullptr);
        glCompileShader(vertexShader);

        int success;
        char message[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, message);
            spdlog::error("Vertex shader compile failed. {}", message);
        } else {
            m_data = std::make_shared<Data>();
            m_data->m_id = vertexShader;
        }
    }
}
