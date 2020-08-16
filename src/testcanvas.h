#pragma once

namespace donut {
    struct TestVertex {
        glm::vec3 m_position;
        glm::vec4 m_color;
    };

	class TestCanvas : public ICanvas {
    public:
        TestCanvas();
        ~TestCanvas();

        void OnAddedToWindow(Window window) override;
        void OnRemovedFromWindow(Window window) override;
        void OnResize(int width, int height) override;
        void OnKey(int key, int action, int mods) override;
        void OnMouseButton(int button, int action, int mods) override;
        void OnMouseScroll(double xOffset, double yOffset) override;
        void OnMouseMove(double x, double y) override;
        void Draw() override;

    private:
        int m_width = 0, m_height = 0;
        Shader m_shader;
        Mesh m_mesh;
	};
}