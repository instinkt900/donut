#pragma once

#include "shadersection.h"

namespace donut::opengl {
    class ShaderImpl : public NonCopyable {
    public:
        static std::shared_ptr<ShaderImpl> Create(std::initializer_list<ShaderSection> const& sections);
        ~ShaderImpl();
        void Bind() const;
        void Unbind() const;
        void SetVector3(std::string const& name, glm::vec3 const& vec);
        void SetVector4(std::string const& name, glm::vec4 const& vec);
        void SetMatrix33(std::string const& name, glm::mat3x3 const& mat);
        void SetMatrix44(std::string const& name, glm::mat4x4 const& mat);

    private:
        int m_id;
        std::unordered_map<std::string, int> m_uniformCache;
        ShaderImpl(int program);
        int GetUniformLoc(std::string const& name);
    };
}