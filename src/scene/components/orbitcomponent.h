#pragma once

namespace donut {
    struct OrbitComponent {
        glm::vec3 m_target;
        float m_length = 1.0f;
        float m_pitch = 0.0f;
        float m_yaw = 0.0f;
        float m_pitchScale = 1.0f;
        float m_yawScale = 1.0f;
    };
}