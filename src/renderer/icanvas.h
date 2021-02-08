#pragma once

#include "window.h"

namespace donut {
    class ICanvas {
    public:
        ICanvas() = default;
        virtual ~ICanvas() = default;

        virtual void OnAddedToWindow(Window* window) = 0;
        virtual void OnRemovedFromWindow(Window* window) = 0;
        virtual void OnResize(int width, int height) = 0;
        virtual void OnKey(int key, int action, int mods) = 0;
        virtual void OnMouseButton(int button, int action, int mods) = 0;
        virtual void OnMouseScroll(double xOffset, double yOffset) = 0;
        virtual void OnMouseMove(double x, double y) = 0;
        virtual void Draw() = 0;
    };
}