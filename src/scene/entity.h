#pragma once

namespace donut {
    class Scene;

	class Entity {
    public:
        Entity() = default;
        Entity(Entity const&) = default;
        Entity& operator=(Entity const&) = default;
        ~Entity() = default;

        bool Valid() const;
        operator bool() const { return Valid(); }

        void Destroy();

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) const {
            assert(Valid());
            return m_scene.lock()->m_registry.emplace<T>(m_id, std::forward<Args>(args)...);
        }

        template<typename... T>
        void RemoveComponent() const {
            assert(Valid());
            m_scene.lock()->m_registry.remove<T...>(m_id);
        }

        template<typename... T>
        bool HasComponent() const {
            assert(Valid());
            return m_scene.lock()->m_registry.has<T...>(m_id);
        }

        template<typename... T>
        decltype(auto) GetComponent() const {
            assert(Valid());
            return m_scene.lock()->m_registry.get<T...>(m_id);
        }

        template<typename... T>
        auto TryGetComponent() const {
            assert(Valid());
            return m_scene.lock()->m_registry.try_get<T...>(m_id);
        }

    private:
        friend Scene;
        std::weak_ptr<Scene> m_scene;
        entt::entity m_id = entt::null;

        Entity(std::shared_ptr<Scene> scene, entt::entity entityId);
	};
}