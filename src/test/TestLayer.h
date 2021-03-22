#pragma once

#include "renderer/events/eventresize.h"
#include "renderer/events/eventkey.h"
#include "renderer/events/eventmousebutton.h"
#include "renderer/events/eventmousescroll.h"
#include "renderer/events/eventmousemove.h"

namespace donut {
    class EmitterSystem;
    class VelocitySystem;
    class LifetimeSystem;
    class InputSystem;
    class CameraSystem;

    struct TestVertex {
        glm::vec3 m_position;
        glm::vec2 m_UV;
    };

    class TestLayer : public ILayer {
    public:
        TestLayer();
        ~TestLayer();

        void OnAddedToStack(LayerStack& stack) override;
        void OnRemovedFromStack(LayerStack& stack) override;
        bool OnEvent(Event const& event) override;
        void Draw() override;

    private:
        int m_width = 0, m_height = 0;
        std::shared_ptr<Scene> m_scene;
        std::unique_ptr<EmitterSystem> m_emitterSystem;
        std::unique_ptr<VelocitySystem> m_velocitySystem;
        std::unique_ptr<LifetimeSystem> m_lifetimeSystem;
        std::unique_ptr<CameraSystem> m_cameraSystem;
        std::unique_ptr<InputSystem> m_inputSystem;
        std::unique_ptr<RenderingSystem> m_renderingSystem;
        std::shared_ptr<FrameBuffer> m_frameBuffer;

        std::chrono::system_clock::time_point m_lastTime;

        bool OnResizeEvent(EventResize const& event);
        bool OnKeyEvent(EventKey const& event);
        bool OnMouseButtonEvent(EventMouseButton const& event);
        bool OnMouseScrollEvent(EventMouseScroll const& event);
        bool OnMouseMoveEvent(EventMouseMove const& event);
    };
}
