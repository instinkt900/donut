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
    std::shared_ptr<donut::Shader> m_shader;
    std::shared_ptr<donut::VertexBuffer> m_vertexBuffer;
    std::shared_ptr<donut::IndexBuffer> m_indexBuffer;
    std::shared_ptr<donut::Texture2D> m_texture;
};
