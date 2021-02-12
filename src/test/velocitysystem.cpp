#include "donut_pch.h"
#include "velocitysystem.h"

#include "scene/components/transformcomponent.h"
#include "velocitycomponent.h"

namespace donut {
    void VelocitySystem::Update(Scene& scene, int timestep) {
        float const seconds = timestep / 1000.0f;
        scene.GetRegistry().view<TransformComponent, VelocityComponent>().each([&](entt::entity entity, TransformComponent& transformComponent, VelocityComponent const& velocityComponent) {
            auto translation = velocityComponent.m_linearVelocity * seconds;
            auto rotation = velocityComponent.m_angularVelocity * seconds;
            transformComponent.m_transform = glm::translate(transformComponent.m_transform, translation);
            transformComponent.m_transform = glm::rotate(transformComponent.m_transform, rotation.x, { 1.0f, 0.0f, 0.0f });
            transformComponent.m_transform = glm::rotate(transformComponent.m_transform, rotation.y, { 0.0f, 1.0f, 0.0f });
            transformComponent.m_transform = glm::rotate(transformComponent.m_transform, rotation.z, { 0.0f, 0.0f, 1.0f });
        });
    }
}