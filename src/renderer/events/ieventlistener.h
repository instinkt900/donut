#pragma once

namespace donut {
    class IEventListener {
    public:
        virtual ~IEventListener() = default;
        virtual bool OnEvent(Event const&) = 0;
    };
}
