#pragma once

namespace donut {
    struct TransformComponent {
        glm::mat4x4 m_transform;

        TransformComponent() = default;
        TransformComponent(glm::mat4x4 const& initial)
            : m_transform(initial) { }
    };
}
