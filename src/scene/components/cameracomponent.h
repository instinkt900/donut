#pragma once

namespace donut {
    struct CameraComponent {
        bool m_active;
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
    };
}
