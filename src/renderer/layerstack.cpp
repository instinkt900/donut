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
    void SafeIterateBreakable(T const& container, F const& func) {
        auto copy = container;
        for (auto&& item : copy) {
            if (func(item))
                break;
        }
    }

    template<typename T, typename F>
    void SafeIterateReverse(T const& container, F const& func) {
        auto copy = container;
        for (auto it = copy.rbegin(); it != copy.rend(); ++it) {
            func(*it);
        }
    }

    template<typename T, typename F>
    void SafeIterateBreakableReverse(T const& container, F const& func) {
        auto copy = container;
        for (auto it = copy.rbegin(); it != copy.rend(); ++it) {
            if (func(*it))
                break;
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

    void LayerStack::OnEvent(Event const& event) {
        SafeIterateBreakableReverse(m_layers, [&](auto& layer) {
            return layer->OnEvent(event);
        });
    }

    void LayerStack::Draw() {
        SafeIterateReverse(m_layers, [&](auto& layer) {
            layer->Draw();
        });
    }
}
