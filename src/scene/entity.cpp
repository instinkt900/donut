#include "donut_pch.h"
#include "entity.h"

namespace donut {
    Entity::Entity(std::shared_ptr<Scene> scene, entt::entity entityId)
        : m_scene(scene), m_id(entityId) {
    }

    bool Entity::Valid() const {
        if (auto scene = m_scene.lock())
            return scene->m_registry.valid(m_id);
        return false;
    }

    void Entity::Destroy() {
        assert(Valid());
        m_scene.lock()->m_registry.destroy(m_id);
        m_id = entt::null;
    }
}
