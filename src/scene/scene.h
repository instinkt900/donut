#pragma once

namespace donut {
    class Entity;

    class Scene : public std::enable_shared_from_this<Scene> {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity();

        entt::registry& GetRegistry() { return m_registry; }

    private:
        friend Entity;
        entt::registry m_registry;
    };
}
