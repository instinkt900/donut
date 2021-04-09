#pragma once

namespace donut {
    class Material {
    public:
        std::shared_ptr<Texture2D> m_texture;
        glm::vec4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
    };
}