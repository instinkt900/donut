#pragma once

namespace donut {
    class EmitterSystem;
    class VelocitySystem;
    class LifetimeSystem;

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
        void OnResize(int width, int height) override;
        void OnKey(int key, int action, int mods) override;
        void OnMouseButton(int button, int action, int mods) override;
        void OnMouseScroll(double xOffset, double yOffset) override;
        void OnMouseMove(double x, double y) override;
        void Draw() override;

    private:
        int m_width = 0, m_height = 0;
        std::shared_ptr<Scene> m_scene;
        std::unique_ptr<EmitterSystem> m_emitterSystem;
        std::unique_ptr<VelocitySystem> m_velocitySystem;
        std::unique_ptr<LifetimeSystem> m_lifetimeSystem;
        std::unique_ptr<RenderingSystem> m_renderingSystem;
        std::shared_ptr<FrameBuffer> m_frameBuffer;

        std::chrono::system_clock::time_point m_lastTime;
    };
}
