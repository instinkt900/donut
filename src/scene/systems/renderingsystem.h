#pragma once

#include "scene/scene.h"

namespace donut {
    class Shader;
    
    class RenderingSystem {
    public:
        RenderingSystem();
        ~RenderingSystem() = default;

        void Update(Scene& scene);

    private:
        std::shared_ptr<Shader> m_shader;
    };
}
