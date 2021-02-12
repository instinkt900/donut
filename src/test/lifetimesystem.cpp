#include "donut_pch.h"
#include "lifetimesystem.h"

#include "lifetimecomponent.h"

namespace donut {
    void LifetimeSystem::Update(Scene& scene, int timestep) {
        std::vector<entt::entity> deadEntities;
        scene.GetRegistry().view<LifetimeComponent>().each([&](entt::entity entity, LifetimeComponent& lifetimeComponent) {
            lifetimeComponent.m_lifetime += timestep;
            if (lifetimeComponent.m_lifetime >= lifetimeComponent.m_maxLifetime) {
                deadEntities.push_back(entity);
            }
        });

        for (auto&& entity : deadEntities) {
            scene.GetRegistry().destroy(entity);
        }
    }
}
