#pragma once

namespace donut {
    class ILayer;

	class LayerStack {
    public:
        LayerStack(IWindow& window);
        ~LayerStack() = default;

        IWindow& GetWindow() const { return m_window; }

        void PushLayer(std::shared_ptr<ILayer> layer);
        void PopLayer();

        void OnResize(int width, int height);
        void OnKey(int key, int action, int mods);
        void OnMouseButton(int button, int action, int mods);
        void OnMouseScroll(double xOffset, double yOffset);
        void OnMouseMove(double x, double y);
        void Draw();

    private:
        IWindow& m_window;
        std::vector<std::shared_ptr<ILayer>> m_layers;
	};
}