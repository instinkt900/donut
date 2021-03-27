#pragma once

namespace donut {
    struct OrbitComponent {
        glm::vec3 m_target;
        std::optional<glm::vec2> m_lastMousePos;
        float m_length = 1.0f;
        float m_pitch = 0.0f;
        float m_yaw = 0.0f;
        float m_pitchScale = 1.0f;
        float m_yawScale = 1.0f;

        static void ImGuiComponent(entt::registry& registry, entt::entity entity) {
            if (auto const* orbitComponent = registry.try_get<OrbitComponent>(entity)) {
                ImGui::PushID(orbitComponent);
                if (ImGui::CollapsingHeader("OrbitComponent")) {
                    ImGui::LabelText("Pitch", "%.2f", orbitComponent->m_pitch);
                    ImGui::LabelText("Yaw", "%.2f", orbitComponent->m_yaw);
                }
                ImGui::PopID();
            }
        }
    };
}