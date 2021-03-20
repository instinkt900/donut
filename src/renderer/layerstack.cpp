#include "donut_pch.h"
#include "layerstack.h"

namespace {
    template<typename T, typename F>
    void SafeIterate(T const& container, F const& func) {
        auto copy = container;
        for (auto&& item : copy) {
            func(item);
        }
    }

    template<typename T, typename F>
    void SafeIterateReverse(T const& container, F const& func) {
        auto copy = container;
        for (auto it = copy.rbegin(); it != copy.rend(); ++it) {
            func(*it);
        }
    }
}

namespace donut {
    LayerStack::LayerStack(IWindow& window)
        : m_window(window) {

    }

    void LayerStack::PushLayer(std::shared_ptr<ILayer> layer) {
        m_layers.insert(m_layers.begin(), layer);
        layer->OnAddedToStack(*this);
    }

    void LayerStack::PopLayer() {
        m_layers.front()->OnRemovedFromStack(*this);
        m_layers.erase(m_layers.begin());
    }

    void LayerStack::OnResize(int width, int height) {
        SafeIterate(m_layers, [&](auto& layer) {
            layer->OnResize(width, height);
        });
    }

    void LayerStack::OnKey(int key, int action, int mods) {
        SafeIterate(m_layers, [&](auto& layer) {
            layer->OnKey(key, action, mods);
        });
    }

    void LayerStack::OnMouseButton(int button, int action, int mods) {
        SafeIterate(m_layers, [&](auto& layer) {
            layer->OnMouseButton(button, action, mods);
        });
    }

    void LayerStack::OnMouseScroll(double xOffset, double yOffset) {
        SafeIterate(m_layers, [&](auto& layer) {
            layer->OnMouseScroll(xOffset, yOffset);
        });
    }

    void LayerStack::OnMouseMove(double x, double y) {
        SafeIterate(m_layers, [&](auto& layer) {
            layer->OnMouseMove(x, y);
        });
    }

    void LayerStack::Draw() {
        SafeIterateReverse(m_layers, [&](auto& layer) {
            layer->Draw();
        });
    }
}
