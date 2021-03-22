#pragma once

namespace donut {
    struct CameraComponent {
        bool m_active;
        glm::mat4x4 m_view;
        glm::mat4x4 m_projection;
    };

    struct PerspectiveProjectionComponent {
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
        bool m_changed = true;
    };
}
