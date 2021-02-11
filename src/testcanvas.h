#pragma once

struct TestVertex {
    glm::vec3 m_position;
    glm::vec2 m_UV;
};

class TestCanvas : public donut::ICanvas {
public:
    TestCanvas();
    ~TestCanvas();

    void OnAddedToWindow(donut::Window* window) override;
    void OnRemovedFromWindow(donut::Window* window) override;
    void OnResize(int width, int height) override;
    void OnKey(int key, int action, int mods) override;
    void OnMouseButton(int button, int action, int mods) override;
    void OnMouseScroll(double xOffset, double yOffset) override;
    void OnMouseMove(double x, double y) override;
    void Draw() override;

private:
    int m_width = 0, m_height = 0;
    std::shared_ptr<donut::Scene> m_scene;
    std::unique_ptr<donut::RenderingSystem> m_renderingSystem;
    std::shared_ptr<donut::FrameBuffer> m_frameBuffer;
};
