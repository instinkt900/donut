#include "donut_pch.h"
#include "camerasystem.h"

#include "scene/components/cameracomponent.h"
#include "scene/components/transformcomponent.h"

namespace donut {
    void CameraSystem::Update(Scene& scene) {
        auto& registry = scene.GetRegistry();
        registry.view<CameraComponent, TransformComponent>().each([&](entt::entity entity, CameraComponent& cameraComponent, TransformComponent const& transformComponent) {
            if (cameraComponent.m_active) {
                cameraComponent.m_view = transformComponent.m_transform;
                if (auto const perspectiveComponent = registry.try_get<PerspectiveProjectionComponent>(entity)) {
                    if (perspectiveComponent->m_changed) {
                        cameraComponent.m_projection = glm::perspective(perspectiveComponent->m_fov, perspectiveComponent->m_aspect, perspectiveComponent->m_near, perspectiveComponent->m_far);
                        perspectiveComponent->m_changed = false;
                    }
                }
            }
        });
    }
}
