#pragma once

namespace donut {
    class ILayer {
    public:
        ILayer() = default;
        virtual ~ILayer() = default;

        virtual void OnAddedToStack(LayerStack& stack) = 0;
        virtual void OnRemovedFromStack(LayerStack& stack) = 0;
        virtual void OnResize(int width, int height) = 0;
        virtual void OnKey(int key, int action, int mods) = 0;
        virtual void OnMouseButton(int button, int action, int mods) = 0;
        virtual void OnMouseScroll(double xOffset, double yOffset) = 0;
        virtual void OnMouseMove(double x, double y) = 0;
        virtual void Draw() = 0;
    };
}