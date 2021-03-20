#pragma once

namespace donut {
    class LayerStack;

    class IWindow {
    public:
        virtual bool Update() = 0;

        virtual LayerStack& GetLayerStack() = 0;

        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;

        virtual int GetContentWidth() const = 0;
        virtual int GetContentHeight() const = 0;
    };
}