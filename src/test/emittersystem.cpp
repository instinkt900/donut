#include "donut_pch.h"
#include "emittersystem.h"

#include "emittercomponent.h"
#include "scene/components/transformcomponent.h"
#include "lifetimecomponent.h"

namespace donut {
    void EmitterSystem::Update(Scene& scene, int timestep) {
        scene.GetRegistry().view<EmitterComponent>().each([&](entt::entity entity, EmitterComponent& emitterComponent) {
            emitterComponent.m_spawnTimer += timestep;
            if (emitterComponent.m_spawnTimer > emitterComponent.m_spawnDelay) {
                emitterComponent.m_spawnTimer -= emitterComponent.m_spawnDelay;

                auto newEntity = scene.CreateEntity();
                newEntity.AddComponent<TransformComponent>(glm::identity<glm::mat4x4>());
                newEntity.AddComponent<VelocityComponent>(emitterComponent.m_initialVelocity);
                auto meshComponent = newEntity.AddComponent<MeshComponent>(emitterComponent.m_mesh);
                newEntity.AddComponent<LifetimeComponent>(LifetimeComponent { emitterComponent.m_lifetime, 0 });
            }
        });
    }
}
