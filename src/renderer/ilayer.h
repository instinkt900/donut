#pragma once

namespace donut {
    class Event;

    class ILayer {
    public:
        ILayer() = default;
        virtual ~ILayer() = default;

        virtual void OnAddedToStack(LayerStack& stack) = 0;
        virtual void OnRemovedFromStack(LayerStack& stack) = 0;

        virtual bool OnEvent(Event const& event) = 0;
        virtual void Draw() = 0;
    };
}