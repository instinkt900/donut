#pragma once

namespace donut {
    class ILayer;
    class Event;

	class LayerStack {
    public:
        LayerStack(IWindow& window);
        ~LayerStack() = default;

        IWindow& GetWindow() const { return m_window; }

        void PushLayer(std::shared_ptr<ILayer> layer);
        void PopLayer();

        void OnEvent(Event const& event);
        void Draw();

    private:
        IWindow& m_window;
        std::vector<std::shared_ptr<ILayer>> m_layers;
	};
}