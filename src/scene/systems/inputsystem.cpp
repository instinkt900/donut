#include "donut_pch.h"
#include "inputsystem.h"

#include "renderer/events/eventdispatch.h"
#include "scene/components/inputcomponent.h"
#include "scene/components/orbitcomponent.h"
#include "scene/components/transformcomponent.h"
#include "renderer/events/eventmousemove.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace {
    float angleWrap(float angle) {
        return static_cast<float>(std::fmod(angle, 2 * M_PI));
    }
}
namespace donut {
    InputSystem::InputSystem(Scene& scene)
        : m_scene(scene) { }

    bool InputSystem::OnEvent(Event const& event) {
        EventDispatch dispatch(event);
        dispatch.Dispatch(this, &InputSystem::OnMouseMoveEvent);
        return dispatch.WasDispatched();
    }

    void InputSystem::Update(Scene& scene) {
    }

    bool InputSystem::OnMouseMoveEvent(EventMouseMove const& mouseMoveEvent) {
        auto& registry = m_scene.GetRegistry();
        registry.view<InputComponent, TransformComponent>().each([&](auto entity, auto const& inputComponent, auto& transformComponent) {
            if (inputComponent.m_enabled) {
                if (auto const orbitComponent = registry.try_get<OrbitComponent>(entity)) {
                    UpdateOrbitComponent(*orbitComponent, transformComponent, mouseMoveEvent);
                }
            }
        });

        return false;
    }

    void InputSystem::UpdateOrbitComponent(OrbitComponent& orbitComponent, TransformComponent& transformComponent, EventMouseMove const& event) const {
        auto const currentPos = glm::vec2(event.GetX(), event.GetY());
        auto const lastPos = orbitComponent.m_lastMousePos.value_or(currentPos);
        auto const deltaPos = currentPos - lastPos;
        orbitComponent.m_yaw = angleWrap(orbitComponent.m_yaw + orbitComponent.m_yawScale * deltaPos.x);
        orbitComponent.m_pitch = angleWrap(orbitComponent.m_pitch + orbitComponent.m_pitchScale * deltaPos.y);
        auto transform = glm::identity<glm::mat4x4>();
        transform = glm::translate(transform, glm::vec3(0, 0, -orbitComponent.m_length));
        transform = glm::rotate(transform, orbitComponent.m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, orbitComponent.m_yaw, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, orbitComponent.m_target);
        transformComponent.m_transform = transform;
        orbitComponent.m_lastMousePos = currentPos;
    }
}
