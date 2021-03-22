#pragma once

#include "renderer/events/ieventlistener.h"

namespace donut {
    struct OrbitComponent;
    struct TransformComponent;
    class EventMouseMove;

    class InputSystem : public IEventListener {
    public:
        InputSystem(Scene& scene);
        ~InputSystem() = default;

        bool OnEvent(Event const& event) override;
        void Update(Scene& scene);

    private:
        Scene& m_scene;

        bool OnMouseMoveEvent(EventMouseMove const& mouseMoveEvent);

        void UpdateOrbitComponent(OrbitComponent& orbitComponent, TransformComponent& transformComponent, EventMouseMove const& event) const;
    };
}
